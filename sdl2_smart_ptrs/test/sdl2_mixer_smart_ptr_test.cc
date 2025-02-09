#if (_CATCH_VERSION_MAJOR == 3)
  #include <catch2/catch_version_macros.hpp>                     // CATCH_VERSION_MAJOR
  #include <catch2/catch_test_macros.hpp>                        // TEST_CASE, SECTION, REQUIRE
#elif (_CATCH_VERSION_MAJOR == 2)
  #include <catch2/catch.hpp>
#endif

#include "sdl2_mixer_smart_ptr.hh"

#include <SDL.h>
#include <SDL_mixer.h>

#include <string>


static std::string collectErrorQuitSdlMix(const std::string& func_name) {
    std::string err { SDL_GetError() };
    SDL_ClearError();
    if (err.size() == 0) {
        err = "failure without setting SDL error";
    }
    Mix_Quit();
    SDL_Quit();
    return func_name + ": " + err;
}

using namespace sdl2_smart_ptr;

TEST_CASE("SDL_mixer allocations: Mix_Chunk",
    "[sdl2_smart_ptr][SDL2][SDL_mixer][Mix_Chunk]")
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SKIP(collectErrorQuitSdlMix("SDL_Init"));
    }

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        FAIL(collectErrorQuitSdlMix("Mix_Init"));
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        SKIP(collectErrorQuitSdlMix("Mix_OpenAudio"));
    }

    Mix_Chunk* chunk {
        // channels are 0-indexed
        // note that freeing chunk will close channel, see:
        //   - https://wiki.libsdl.org/SDL2_mixer/Mix_FreeChunk
        Mix_GetChunk(1)
    };
    if (chunk == nullptr) {
        Mix_CloseAudio();
        SKIP(collectErrorQuitSdlMix("Mix_GetChunk"));
    }

    deleter::MixChunk dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(chunk);
    }
    SECTION("unique:: ctor")
    {
        unique::MixChunk up_chunk{chunk, dltr};
        REQUIRE(up_chunk.get() == chunk);
    }
    SECTION("make_unique")
    {
        auto up_chunk{ make_unique(chunk) };
        REQUIRE(up_chunk.get() == chunk);
    }
    SECTION("shared:: ctor")
    {
        shared::MixChunk sp_chunk{chunk, dltr};
        REQUIRE(sp_chunk.get() == chunk);
    }
    SECTION("make_shared")
    {
        auto sp_chunk{ make_shared(chunk) };
        REQUIRE(sp_chunk.get() == chunk);
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

TEST_CASE("SDL_mixer allocations: Mix_Music",
    "[sdl2_smart_ptr][SDL2][SDL_mixer][Mix_Music]")
{
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        SKIP(collectErrorQuitSdlMix("SDL_Init"));
    }

    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        FAIL(collectErrorQuitSdlMix("Mix_Init"));
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        SKIP(collectErrorQuitSdlMix("Mix_OpenAudio"));
    }

    Mix_Music* music {
        Mix_LoadMUS(EXAMPLE_DATA_DIR "2A.mp3")
    };
    if (music == nullptr) {
        Mix_CloseAudio();
        SKIP(collectErrorQuitSdlMix("Mix_LoadMUS"));
    }

    deleter::MixMusic dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(music);
    }
    SECTION("unique:: ctor")
    {
        unique::MixMusic up_music{music, dltr};
        REQUIRE(up_music.get() == music);
    }
    SECTION("make_unique")
    {
        auto up_music{ make_unique(music) };
        REQUIRE(up_music.get() == music);
    }
    SECTION("shared:: ctor")
    {
        shared::MixMusic sp_music{music, dltr};
        REQUIRE(sp_music.get() == music);
    }
    SECTION("make_shared")
    {
        auto sp_music{ make_shared(music) };
        REQUIRE(sp_music.get() == music);
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}
