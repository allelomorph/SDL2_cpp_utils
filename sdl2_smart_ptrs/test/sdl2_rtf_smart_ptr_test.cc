#if (_CATCH_VERSION_MAJOR == 3)
  #include <catch2/catch_version_macros.hpp>                     // CATCH_VERSION_MAJOR
  #include <catch2/catch_test_macros.hpp>                        // TEST_CASE, SECTION, REQUIRE
#elif (_CATCH_VERSION_MAJOR == 2)
  #include <catch2/catch.hpp>
#endif

#include "sdl2_rtf_smart_ptr.hh"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_rtf.h>

#include <string>


static std::string collectErrorQuitSdlTtf(const std::string& func_name) {
    std::string err { SDL_GetError() };
    SDL_ClearError();
    if (err.size() == 0) {
        err = "failure without setting SDL error";
    }
    TTF_Quit();
    SDL_Quit();
    return func_name + ": " + err;
}

using namespace sdl2_smart_ptr;

TEST_CASE("SDL_rtf allocations: RTF_Context",
    "[sdl2_smart_ptr][SDL2][SDL_rtf][RTF_Context]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdlTtf("SDL_Init"));
    }

    if (TTF_Init() != 0) {
        FAIL(collectErrorQuitSdlTtf("TTF_Init"));
    }

    SDL_Window* window {
        SDL_CreateWindow("test_window", 0, 0, 1, 1, SDL_WINDOW_HIDDEN)
    };
    if (window == nullptr) {
        FAIL(collectErrorQuitSdlTtf("SDL_CreateWindow"));
    }

    SDL_Renderer* renderer {
        SDL_CreateRenderer(window, -1, 0)
    };
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        FAIL(collectErrorQuitSdlTtf("SDL_CreateRenderer"));
    }

    RTF_FontEngine font_engine {};
    font_engine.version = RTF_FONT_ENGINE_VERSION;

    RTF_Context* context {
        RTF_CreateContext(renderer, &font_engine)
    };
    if (context == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SKIP(collectErrorQuitSdlTtf("RTF_CreateContext"));
    }

    deleter::RtfContext dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(context);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::RtfContext up_context{context, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_context{ make_unique(context) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::RtfContext sp_context{context, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_context{ make_shared(context) };
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
