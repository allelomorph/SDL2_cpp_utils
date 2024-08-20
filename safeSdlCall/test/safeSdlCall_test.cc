#if (_CATCH_VERSION_MAJOR == 3)
  #include <catch2/catch_version_macros.hpp>                     // CATCH_VERSION_MAJOR
  #include <catch2/catch_test_macros.hpp>                        // TEST_CASE, SECTION, REQUIRE
  #include <catch2/matchers/catch_matchers.hpp>                  // REQUIRES_*THROW*
  #include <catch2/matchers/catch_matchers_exception.hpp>        // Catch::Matchers::Message
  #include <catch2/reporters/catch_reporter_event_listener.hpp>  // Catch::EventListenerBase
  #include <catch2/reporters/catch_reporter_registrars.hpp>      // CATCH_REGISTER_LISTENER
#elif (_CATCH_VERSION_MAJOR == 2)
  #include <catch2/catch.hpp>
#endif

#include "safeSdlCall.hh"

#if (CATCH_VERSION_MAJOR > 2)
using Catch::Matchers::Message;
#else
using Catch::Message;
#endif


TEST_CASE("SDL core function",
    "[SDL2, core]")
{
    REQUIRE(SDL_Init(SDL_INIT_VIDEO) == 0);

    SdlRetTest<Uint32> sdl_getwindowid_test {
        [](const Uint32 ret){ return (ret == 0); }
    };

    SECTION("Success")
    {
        SDL_Window* window {
            SDL_CreateWindow("sdl_test_window", 0, 0, 1, 1, SDL_WINDOW_HIDDEN)
        };
        REQUIRE(window != nullptr);

        REQUIRE_NOTHROW(
            safeSdlCall(
                SDL_GetWindowID, "SDL_GetWindowID", sdl_getwindowid_test,
                window) == 1
           );

        SDL_DestroyWindow(window);
    }
    SECTION("Failure")
    {
        SECTION("SDL_net API sets error")
        {
            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    SDL_GetWindowID, "SDL_GetWindowID", sdl_getwindowid_test,
                    nullptr) == 0,
                std::runtime_error,
                Message("SDL_GetWindowID: Invalid window")
                );
            REQUIRE(std::string_view(SDL_GetError()) == "Invalid window");
        }
        /*
TBD find SDL core function that fails without calling SDL_SetError
        SECTION("SDL_net API does not set error")
        {
        }
        */
    }

    SDL_Quit();
}

TEST_CASE("SDL_image core function",
    "[SDL2, SDL_image]")
{
    REQUIRE(SDL_Init(SDL_INIT_VIDEO) == 0);

    SECTION("Success")
    {
//        REQUIRE_NOTHROW(
//           );
    }
    SECTION("Failure")
    {
//        REQUIRE_THROWS_MATCHES(
//            );
    }

    SDL_Quit();
}

TEST_CASE("SDL_mixer function",
    "[SDL2, SDL_mixer]")
{
    SECTION("Success")
    {
//        REQUIRE_NOTHROW(
//           );
    }
    SECTION("Failure")
    {
//        REQUIRE_THROWS_MATCHES(
//            );
    }
}

TEST_CASE("SDL_ttf function",
    "[SDL2, SDL_ttf]")
{
    SECTION("Success")
    {
//        REQUIRE_NOTHROW(
//           );
    }
    SECTION("Failure")
    {
//        REQUIRE_THROWS_MATCHES(
//            );
    }
}

TEST_CASE("SDL_rtf function",
    "[SDL2, SDL_rtf]")
{
    SECTION("Success")
    {
//        REQUIRE_NOTHROW(
//           );
    }
    SECTION("Failure")
    {
//        REQUIRE_THROWS_MATCHES(
//            );
    }
}

TEST_CASE("SDL_net function",
    "[SDL2, SDL_net]")
{
    REQUIRE(SDL_Init(SDL_INIT_VIDEO) == 0);
    REQUIRE(SDLNet_Init() == 0);

    SdlRetTest<const int> sdlnet_resolvehost_test {
        [](const int ret){ return (ret == -1); }
    };

    SECTION("Success")
    {
        IPaddress address;
        REQUIRE_NOTHROW(
            safeSdlCall(
                SDLNet_ResolveHost, "SDLNet_ResolveHost", sdlnet_resolvehost_test,
                &address, "localhost", 0
                ) == 0
            );
    }
    SECTION("Failure")
    {
/*
TBD: find a SDL_net function that alters state as little as possible but also
  calls SDLNet_SetError
        SECTION("SDL_net API sets error")
        {
        }
*/
        SECTION("SDL_net API does not set error")
        {
            IPaddress address;
            REQUIRE_THROWS_MATCHES(
                safeSdlCall(
                    SDLNet_ResolveHost, "SDLNet_ResolveHost", sdlnet_resolvehost_test,
                    &address, "", 0
                    ) == -1,
                std::runtime_error,
                Message("SDLNet_ResolveHost: ")
                );
        }
    }

    SDLNet_Quit();
    SDL_Quit();
}
