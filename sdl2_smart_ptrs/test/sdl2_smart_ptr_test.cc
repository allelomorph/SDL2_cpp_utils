#if (_CATCH_VERSION_MAJOR == 3)
  #include <catch2/catch_version_macros.hpp>                     // CATCH_VERSION_MAJOR
  #include <catch2/catch_test_macros.hpp>                        // TEST_CASE, SECTION, REQUIRE
  #include <catch2/matchers/catch_matchers.hpp>                  // REQUIRES_*THROW*
  #include <catch2/matchers/catch_matchers_exception.hpp>        // Catch::Matchers::Message
#elif (_CATCH_VERSION_MAJOR == 2)
  #include <catch2/catch.hpp>
#endif

#include "sdl2_smart_ptr.hh"

#include <SDL.h>

#include <string>


std::string collectErrorQuitSdl(const std::string& func_name) {
    std::string err { SDL_GetError() };
    SDL_ClearError();
    if (err.size() == 0) {
        err = "failure without setting SDL error";
    }
    SDL_Quit();
    return func_name + ": " + err;
}

#if (CATCH_VERSION_MAJOR > 2)
using Catch::Matchers::Message;
#else
using Catch::Message;
#endif

using namespace sdl2_smart_ptr;

TEST_CASE("SDL core allocations: SDL_Cursor",
    "[sdl2_smart_ptr][SDL2][core][SDL_Cursor]")
{
    int init_ret { SDL_Init(SDL_INIT_VIDEO) };
    if (init_ret != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    SDL_Cursor* cursor {
        SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW)
    };
    if (cursor == nullptr) {
        SKIP(collectErrorQuitSdl("SDL_CreateSystemCursor"));
    }

    deleter::Cursor dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(cursor);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::Cursor up_cursor{cursor, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_cursor{ make_unique(cursor) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::Cursor sp_cursor{cursor, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_cursor{ make_shared(cursor) };
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_cond",
    "[sdl2_smart_ptr][SDL2][core][SDL_cond]")
{
    int init_ret { SDL_Init(SDL_INIT_VIDEO) };
    if (init_ret != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    SDL_cond* cond {
        SDL_CreateCond()
    };
    if (cond == nullptr) {
        SKIP(collectErrorQuitSdl("SDL_CreateCond"));
    }

    deleter::CondVar dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(cond);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::CondVar up_cond{cond, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_cond{ make_unique(cond) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::CondVar sp_cond{cond, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_cond{ make_shared(cond) };
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_mutex",
    "[sdl2_smart_ptr][SDL2][core][SDL_mutex]")
{
    int init_ret { SDL_Init(SDL_INIT_VIDEO) };
    if (init_ret != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    SDL_mutex* mutex {
        SDL_CreateMutex()
    };
    if (mutex == nullptr) {
        SKIP(collectErrorQuitSdl("SDL_CreateMutex"));
    }

    deleter::Mutex dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(mutex);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::Mutex up_mutex{mutex, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_mutex{ make_unique(mutex) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::Mutex sp_mutex{mutex, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_mutex{ make_shared(mutex) };
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_Renderer",
    "[sdl2_smart_ptr][SDL2][core][SDL_Renderer]")
{
    int init_ret { SDL_Init(SDL_INIT_VIDEO) };
    if (init_ret != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    SDL_Window* window {
        SDL_CreateWindow("test_window", 0, 0, 1, 1, SDL_WINDOW_HIDDEN)
    };
    if (window == nullptr) {
        FAIL(collectErrorQuitSdl("SDL_CreateWindow"));
    }

    SDL_Renderer* renderer {
        SDL_CreateRenderer(window, -1, 0)
    };
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        FAIL(collectErrorQuitSdl("SDL_CreateRenderer"));
    }

    deleter::Renderer dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(renderer);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::Renderer up_renderer{renderer, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_renderer{ make_unique(renderer) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::Renderer sp_renderer{renderer, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_renderer{ make_shared(renderer) };
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_sem",
    "[sdl2_smart_ptr][SDL2][core][SDL_sem]")
{
    int init_ret { SDL_Init(SDL_INIT_VIDEO) };
    if (init_ret != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    SDL_sem* semaphore {
        SDL_CreateSemaphore(1)
    };
    if (semaphore == nullptr) {
        SKIP(collectErrorQuitSdl("SDL_CreateSemaphore"));
    }

    deleter::Semaphore dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(semaphore);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::Semaphore up_semaphore{semaphore, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_semaphore{ make_unique(semaphore) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::Semaphore sp_semaphore{semaphore, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_semaphore{ make_shared(semaphore) };
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_Surface",
    "[sdl2_smart_ptr][SDL2][core][SDL_Surface]")
{
    int init_ret { SDL_Init(SDL_INIT_VIDEO) };
    if (init_ret != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    SDL_Surface* surface {
        SDL_CreateRGBSurface(0, 1, 1, 32, 0, 0, 0, 0)
    };
    if (surface == nullptr) {
        FAIL(collectErrorQuitSdl("SDL_CreateRGBSurface"));
    }

    deleter::Surface dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(surface);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::Surface up_surface{surface, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_surface{ make_unique(surface) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::Surface sp_surface{surface, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_surface{ make_shared(surface) };
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_Texture",
    "[sdl2_smart_ptr][SDL2][core][SDL_Texture]")
{
    int init_ret { SDL_Init(SDL_INIT_VIDEO) };
    if (init_ret != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    SDL_Window* window {
        SDL_CreateWindow("test_window", 0, 0, 1, 1, SDL_WINDOW_HIDDEN)
    };
    if (window == nullptr) {
        FAIL(collectErrorQuitSdl("SDL_CreateWindow"));
    }

    SDL_Renderer* renderer {
        SDL_CreateRenderer(window, -1, 0)
    };
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        FAIL(collectErrorQuitSdl("SDL_CreateRenderer"));
    }

    SDL_Texture* texture {
        SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                          SDL_TEXTUREACCESS_STATIC, 1, 1)
    };
    if (texture == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        FAIL(collectErrorQuitSdl("SDL_CreateTexture"));
    }

    deleter::Texture dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(texture);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::Texture up_texture{texture, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_texture{ make_unique(texture) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::Texture sp_texture{texture, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_texture{ make_shared(texture) };
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_Window",
    "[sdl2_smart_ptr][SDL2][core][SDL_Window]")
{
    int init_ret { SDL_Init(SDL_INIT_VIDEO) };
    if (init_ret != 0) {
        FAIL(collectErrorQuitSdl("SDL_Init"));
    }

    SDL_Window* window {
        SDL_CreateWindow("test_window", 0, 0, 1, 1, SDL_WINDOW_HIDDEN)
    };
    if (window == nullptr) {
        FAIL(collectErrorQuitSdl("SDL_CreateWindow"));
    }

    deleter::Window dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(window);
    }
    SECTION("unique:: ctor")
    {
        [[maybe_unused]] unique::Window up_window{window, dltr};
    }
    SECTION("make_unique")
    {
        [[maybe_unused]] auto up_window{ make_unique(window) };
    }
    SECTION("shared:: ctor")
    {
        [[maybe_unused]] shared::Window sp_window{window, dltr};
    }
    SECTION("make_shared")
    {
        [[maybe_unused]] auto sp_window{ make_shared(window) };
    }

    SDL_Quit();
}
