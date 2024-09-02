#if (_CATCH_VERSION_MAJOR == 3)
  #include <catch2/catch_version_macros.hpp>                     // CATCH_VERSION_MAJOR
  #include <catch2/catch_test_macros.hpp>                        // TEST_CASE, SECTION, REQUIRE
  #include <catch2/matchers/catch_matchers.hpp>                  // REQUIRES_*THROW*
  #include <catch2/matchers/catch_matchers_exception.hpp>        // Catch::Matchers::Message
#elif (_CATCH_VERSION_MAJOR == 2)
  #include <catch2/catch.hpp>
#endif

#include "safeSdlCall.hh"

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_rtf.h>

#include <string>


static std::string collectErrorQuitSdl(const std::string& func_name,
                                       void (*lib_quit_func)() = nullptr) {
    std::string err { SDL_GetError() };
    SDL_ClearError();
    if (err.size() == 0) {
        err = "failure without setting SDL error";
    }
    if (lib_quit_func != nullptr) {
        lib_quit_func();
    }
    SDL_Quit();
    return func_name + ": " + err;
}

static bool sdl_getwindowid_test_func(const Uint32 ret) {
    return (ret == 0);
}

#if (CATCH_VERSION_MAJOR > 2)
using Catch::Matchers::Message;
#else
using Catch::Message;
#endif

TEST_CASE("SDL core function",
    "[safeSdlCall][SDL2][core]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    auto sdl_getwindowid_test_lambda {
        [](const Uint32 ret){ return (ret == 0); }
    };

    SECTION("Success")
    {
        SDL_Window* window {
            SDL_CreateWindow("sdl_test_window", 0, 0, 1, 1, SDL_WINDOW_HIDDEN)
        };
        if (window == nullptr) {
            FAIL(collectErrorQuitSdl("SDL_CreateWindow"));
        }

        SECTION("detected by function") {
            REQUIRE_NOTHROW(
                safeSdlCall(
                    SDL_GetWindowID, "SDL_GetWindowID",
                    SdlRetTest<Uint32>{ sdl_getwindowid_test_func },
                    window) == 1
                );
        }
        SECTION("detected by functor") {
            struct sdl_getwindowid_test_functor {
                bool operator()(const Uint32 ret) { return (ret == 0); }
            };

            REQUIRE_NOTHROW(
                safeSdlCall(
                    SDL_GetWindowID, "SDL_GetWindowID",
                    SdlRetTest<Uint32>{ sdl_getwindowid_test_functor() },
                    window) == 1
                );
        }
        SECTION("detected by lambda") {
            REQUIRE_NOTHROW(
                safeSdlCall(
                    SDL_GetWindowID, "SDL_GetWindowID",
                    SdlRetTest<Uint32>{ sdl_getwindowid_test_lambda },
                    window) == 1
                );
        }

        SDL_DestroyWindow(window);
    }
    SECTION("Failure")
    {
        SECTION("with SDL error set")
        {
            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    SDL_GetWindowID, "SDL_GetWindowID",
                    SdlRetTest<Uint32>{ sdl_getwindowid_test_lambda },
                    nullptr) == 0,
                std::runtime_error,
                Message("SDL_GetWindowID: Invalid window")
                );
        }
        // SECTION("with no SDL error set")
        // {
        // }
    }

    SDL_Quit();
}

TEST_CASE("SDL_image function",
    "[safeSdlCall][SDL2][SDL_image]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    if (IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG) {
        FAIL(collectErrorQuitSdl("SDL_Init", IMG_Quit));
    }

    SdlRetTest<SDL_Surface*> img_load_test {
        [](const SDL_Surface* ret){ return (ret == nullptr); }
    };

    SECTION("Success")
    {
        // string literal EXAMPLE_DATA_DIR should be defined by cmake
        SDL_Surface* surface {};
        REQUIRE_NOTHROW(
            (surface = safeSdlCall(
                IMG_Load, "IMG_Load", img_load_test,
                EXAMPLE_DATA_DIR "privat_parkering.jpg"
                )) != nullptr
            );
        SDL_FreeSurface(surface);
    }
    SECTION("Failure")
    {
        SECTION("with SDL error set")
        {
            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    IMG_Load, "IMG_Load", img_load_test,
                    "") == nullptr,
                std::runtime_error,
                Message("IMG_Load: SDL_RWFromFile(): No file or no mode specified")
                );
        }
        // SECTION("with no SDL error set")
        // {
        // }
    }

    IMG_Quit();
    SDL_Quit();
}

TEST_CASE("SDL_mixer function",
    "[safeSdlCall][SDL2][SDL_mixer]")
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SKIP(collectErrorQuitSdl("SDL_Init"));
    }

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        FAIL(collectErrorQuitSdl("Mix_Init", Mix_Quit));
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        SKIP(collectErrorQuitSdl("Mix_OpenAudio", Mix_Quit));
    }

    SdlRetTest<Mix_Music*> mix_loadmus_test {
        [](const Mix_Music* ret){ return (ret == nullptr); }
    };

    SECTION("Success")
    {
        Mix_Music* music {};
        REQUIRE_NOTHROW(
            (music = safeSdlCall(
                Mix_LoadMUS, "Mix_LoadMUS", mix_loadmus_test,
                EXAMPLE_DATA_DIR "2A.mp3"
                )) != nullptr
            );
        Mix_FreeMusic(music);
    }
    SECTION("Failure")
    {
        SECTION("with SDL error set")
        {
            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    Mix_LoadMUS, "Mix_LoadMUS", mix_loadmus_test,
                    "") == nullptr,
                std::runtime_error,
                Message("Mix_LoadMUS: Couldn't open ''")
                );
        }
        // SECTION("with no SDL error set")
        // {
        // }
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

TEST_CASE("SDL_ttf function",
    "[safeSdlCall][SDL2][SDL_ttf]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    if (TTF_Init() != 0) {
        FAIL(collectErrorQuitSdl("TTF_Init", TTF_Quit));
    }

    SdlRetTest<TTF_Font*> ttf_openfont_test {
        [](const TTF_Font* ret){ return (ret == nullptr); }
    };

    SECTION("Success")
    {
        // string literal EXAMPLE_DATA_DIR should be defined by cmake
        TTF_Font* font {};
        REQUIRE_NOTHROW(
            (font = safeSdlCall(
                TTF_OpenFont, "TTF_OpenFont", ttf_openfont_test,
                EXAMPLE_DATA_DIR "Courier New.ttf", 8
                )) != nullptr
            );
        TTF_CloseFont(font);
    }
    SECTION("Failure")
    {
        SECTION("with SDL error set")
        {
            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    TTF_OpenFont, "TTF_OpenFont", ttf_openfont_test,
                    "", 8) == nullptr,
                std::runtime_error,
                Message("TTF_OpenFont: SDL_RWFromFile(): No file or no mode specified")
                );
        }
        SECTION("with no SDL error set")
        {
            SdlRetTest<int> ttf_setfontdirection_test {
                [](const int ret){ return (ret == -1); }
            };

            REQUIRE_THROWS_MATCHES(
                // out of enum range TTF_Direction value returns failure
                //   regardless of Harfbuzz support
                safeSdlCall(
                    TTF_SetFontDirection, "TTF_SetFontDirection", ttf_setfontdirection_test,
                    nullptr, TTF_Direction(100)) == -1,
                std::runtime_error,
                Message("TTF_SetFontDirection: failure without setting SDL error")
                );
        }
    }

    TTF_Quit();
    SDL_Quit();
}

TEST_CASE("SDL_rtf function",
    "[safeSdlCall][SDL2][SDL_rtf]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    if (TTF_Init() != 0) {
        FAIL(collectErrorQuitSdl("TTF_Init", TTF_Quit));
    }

    SDL_Window* window {
        SDL_CreateWindow("test_window", 0, 0, 1, 1, SDL_WINDOW_HIDDEN)
    };
    if (window == nullptr) {
        FAIL(collectErrorQuitSdl("SDL_CreateWindow", TTF_Quit));
    }

    SDL_Renderer* renderer {
        SDL_CreateRenderer(window, -1, 0)
    };
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        FAIL(collectErrorQuitSdl("SDL_CreateRenderer", TTF_Quit));
    }

    RTF_FontEngine font_engine {};
    font_engine.version = RTF_FONT_ENGINE_VERSION;

    SdlRetTest<RTF_Context*> rtf_createcontext_test {
        [](const RTF_Context* ret){ return (ret == nullptr); }
    };

    SECTION("Success")
    {
        RTF_Context* rtf_context {};
        REQUIRE_NOTHROW(
            (rtf_context = safeSdlCall(
                RTF_CreateContext, "RTF_CreateContext", rtf_createcontext_test,
                renderer, &font_engine
                )) != nullptr
            );
        RTF_FreeContext(rtf_context);
    }
    SECTION("Failure")
    {
        SECTION("with SDL error set")
        {
            font_engine.version = 0;
            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    RTF_CreateContext, "RTF_CreateContext", rtf_createcontext_test,
                    renderer, &font_engine
                    ),
                std::runtime_error,
                Message("RTF_CreateContext: Unknown font engine version")
                );
        }
        // SECTION("with no SDL error set")
        // {
        // }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

TEST_CASE("SDL_net function",
    "[safeSdlCall][SDL2][SDL_net]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    if (SDLNet_Init() != 0) {
        FAIL(collectErrorQuitSdl("SDLNet_Init", SDLNet_Quit));
    }

    SdlRetTest<int> sdlnet_resolvehost_test {
        [](const int ret){ return (ret == -1); }
    };

    IPaddress address;

    SECTION("Success")
    {
        REQUIRE_NOTHROW(
            safeSdlCall(
                SDLNet_ResolveHost, "SDLNet_ResolveHost", sdlnet_resolvehost_test,
                &address, "localhost", 0
                ) == 0
            );
    }
    SECTION("Failure")
    {
        SECTION("with SDL error set")
        {
            if (SDLNet_ResolveHost(&address, "localhost", 0) != 0) {
                FAIL(collectErrorQuitSdl("SDLNet_ResolveHost", SDLNet_Quit));
            }

            SdlRetTest<TCPsocket> sdlnet_tcp_open_test {
                [](const TCPsocket ret){ return (ret == nullptr); },
            };

            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    SDLNet_TCP_Open, "SDLNet_TCP_Open", sdlnet_tcp_open_test,
                    &address
                    ) == nullptr,
                std::runtime_error,
                Message("SDLNet_TCP_Open: Couldn't connect to remote host")
                );
        }
        SECTION("with no SDL error set")
        {
            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    SDLNet_ResolveHost, "SDLNet_ResolveHost", sdlnet_resolvehost_test,
                    &address, "", 0
                    ) == -1,
                std::runtime_error,
                Message("SDLNet_ResolveHost: failure without setting SDL error")
                );
        }
    }

    SDLNet_Quit();
    SDL_Quit();
}
