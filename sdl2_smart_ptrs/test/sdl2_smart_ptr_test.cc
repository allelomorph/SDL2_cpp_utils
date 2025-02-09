#if (_CATCH_VERSION_MAJOR == 3)
  #include <catch2/catch_version_macros.hpp>                     // CATCH_VERSION_MAJOR
  #include <catch2/catch_test_macros.hpp>                        // TEST_CASE, SECTION, REQUIRE
#elif (_CATCH_VERSION_MAJOR == 2)
  #include <catch2/catch.hpp>
#endif

#include "sdl2_smart_ptr.hh"

#include <SDL.h>

#include <string>


static std::string collectErrorQuitSdl(const std::string& func_name) {
    std::string err { SDL_GetError() };
    SDL_ClearError();
    if (err.size() == 0) {
        err = "failure without setting SDL error";
    }
    SDL_Quit();
    return func_name + ": " + err;
}

using namespace sdl2_smart_ptr;

TEST_CASE("SDL core allocations: SDL_Cursor",
    "[sdl2_smart_ptr][SDL2][core][SDL_Cursor]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        unique::Cursor up_cursor{cursor, dltr};
        REQUIRE(up_cursor.get() == cursor);
    }
    SECTION("make_unique")
    {
        auto up_cursor{ make_unique(cursor) };
        REQUIRE(up_cursor.get() == cursor);
    }
    SECTION("shared:: ctor")
    {
        shared::Cursor sp_cursor{cursor, dltr};
        REQUIRE(sp_cursor.get() == cursor);
    }
    SECTION("make_shared")
    {
        auto sp_cursor{ make_shared(cursor) };
        REQUIRE(sp_cursor.get() == cursor);
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_cond",
    "[sdl2_smart_ptr][SDL2][core][SDL_cond]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        unique::CondVar up_cond{cond, dltr};
        REQUIRE(up_cond.get() == cond);
    }
    SECTION("make_unique")
    {
        auto up_cond{ make_unique(cond) };
        REQUIRE(up_cond.get() == cond);
    }
    SECTION("shared:: ctor")
    {
        shared::CondVar sp_cond{cond, dltr};
        REQUIRE(sp_cond.get() == cond);
    }
    SECTION("make_shared")
    {
        auto sp_cond{ make_shared(cond) };
        REQUIRE(sp_cond.get() == cond);
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_mutex",
    "[sdl2_smart_ptr][SDL2][core][SDL_mutex]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        unique::Mutex up_mutex{mutex, dltr};
        REQUIRE(up_mutex.get() == mutex);
    }
    SECTION("make_unique")
    {
        auto up_mutex{ make_unique(mutex) };
        REQUIRE(up_mutex.get() == mutex);
    }
    SECTION("shared:: ctor")
    {
        shared::Mutex sp_mutex{mutex, dltr};
        REQUIRE(sp_mutex.get() == mutex);
    }
    SECTION("make_shared")
    {
        auto sp_mutex{ make_shared(mutex) };
        REQUIRE(sp_mutex.get() == mutex);
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_Renderer",
    "[sdl2_smart_ptr][SDL2][core][SDL_Renderer]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        unique::Renderer up_renderer{renderer, dltr};
        REQUIRE(up_renderer.get() == renderer);
    }
    SECTION("make_unique")
    {
        auto up_renderer{ make_unique(renderer) };
        REQUIRE(up_renderer.get() == renderer);
    }
    SECTION("shared:: ctor")
    {
        shared::Renderer sp_renderer{renderer, dltr};
        REQUIRE(sp_renderer.get() == renderer);
    }
    SECTION("make_shared")
    {
        auto sp_renderer{ make_shared(renderer) };
        REQUIRE(sp_renderer.get() == renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_sem",
    "[sdl2_smart_ptr][SDL2][core][SDL_sem]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        unique::Semaphore up_semaphore{semaphore, dltr};
        REQUIRE(up_semaphore.get() == semaphore);
    }
    SECTION("make_unique")
    {
        auto up_semaphore{ make_unique(semaphore) };
        REQUIRE(up_semaphore.get() == semaphore);
    }
    SECTION("shared:: ctor")
    {
        shared::Semaphore sp_semaphore{semaphore, dltr};
        REQUIRE(sp_semaphore.get() == semaphore);
    }
    SECTION("make_shared")
    {
        auto sp_semaphore{ make_shared(semaphore) };
        REQUIRE(sp_semaphore.get() == semaphore);
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_Surface",
    "[sdl2_smart_ptr][SDL2][core][SDL_Surface]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        unique::Surface up_surface{surface, dltr};
        REQUIRE(up_surface.get() == surface);
    }
    SECTION("make_unique")
    {
        auto up_surface{ make_unique(surface) };
        REQUIRE(up_surface.get() == surface);
    }
    SECTION("shared:: ctor")
    {
        shared::Surface sp_surface{surface, dltr};
        REQUIRE(sp_surface.get() == surface);
    }
    SECTION("make_shared")
    {
        auto sp_surface{ make_shared(surface) };
        REQUIRE(sp_surface.get() == surface);
    }

    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_Texture",
    "[sdl2_smart_ptr][SDL2][core][SDL_Texture]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        unique::Texture up_texture{texture, dltr};
        REQUIRE(up_texture.get() == texture);
    }
    SECTION("make_unique")
    {
        auto up_texture{ make_unique(texture) };
        REQUIRE(up_texture.get() == texture);
    }
    SECTION("shared:: ctor")
    {
        shared::Texture sp_texture{texture, dltr};
        REQUIRE(sp_texture.get() == texture);
    }
    SECTION("make_shared")
    {
        auto sp_texture{ make_shared(texture) };
        REQUIRE(sp_texture.get() == texture);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

TEST_CASE("SDL core allocations: SDL_Window",
    "[sdl2_smart_ptr][SDL2][core][SDL_Window]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
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
        unique::Window up_window{window, dltr};
        REQUIRE(up_window.get() == window);
    }
    SECTION("make_unique")
    {
        auto up_window{ make_unique(window) };
        REQUIRE(up_window.get() == window);
    }
    SECTION("shared:: ctor")
    {
        shared::Window sp_window{window, dltr};
        REQUIRE(sp_window.get() == window);
    }
    SECTION("make_shared")
    {
        auto sp_window{ make_shared(window) };
        REQUIRE(sp_window.get() == window);
    }

    SDL_Quit();
}
