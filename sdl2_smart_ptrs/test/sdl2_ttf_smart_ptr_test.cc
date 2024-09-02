#if (_CATCH_VERSION_MAJOR == 3)
  #include <catch2/catch_version_macros.hpp>                     // CATCH_VERSION_MAJOR
  #include <catch2/catch_test_macros.hpp>                        // TEST_CASE, SECTION, REQUIRE
#elif (_CATCH_VERSION_MAJOR == 2)
  #include <catch2/catch.hpp>
#endif

#include "sdl2_ttf_smart_ptr.hh"

#include <SDL.h>
#include <SDL_ttf.h>

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

TEST_CASE("SDL_ttf allocations: TTF_Font",
    "[sdl2_smart_ptr][SDL2][SDL_ttf][TTF_Font]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdlTtf("SDL_Init"));
    }

    if (TTF_Init() != 0) {
        FAIL(collectErrorQuitSdlTtf("TTF_Init"));
    }

    TTF_Font* ttf_font {
        TTF_OpenFont(EXAMPLE_DATA_DIR "Courier New.ttf", 10)
    };
    if (ttf_font == nullptr) {
        SKIP(collectErrorQuitSdlTtf("TTF_OpenFont"));
    }

    deleter::TtfFont dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(ttf_font);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::TtfFont up_ttf_font{ttf_font, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_ttf_font{ make_unique(ttf_font) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::TtfFont sp_ttf_font{ttf_font, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_ttf_font{ make_shared(ttf_font) };
    }

    TTF_Quit();
    SDL_Quit();
}
