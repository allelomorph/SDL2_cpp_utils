# newest features used: FetchContent v3.11, FetchContent_MakeAvailable v3.14,
#   CMAKE_FIND_PACKAGE_PREFER_CONFIG v3.15
cmake_minimum_required(VERSION 3.16)

# cmake 3.16-supplied FindSDL*.cmake modules are for SDL1.x, so to use SDL2 we
#   need to supply our own find modules

include(FetchIfNotFound)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)

# See https://packages.ubuntu.com/search?keywords=sdl2 for package versions
# In all cases currently only considering SDL2 v2, not v3

# Relevant SDL2 releases:
# - release matching Ubuntu 20.04 LTS package:
#   2.0.10 0e9560aea22818884921e5e5064953257bfe7fa7
# - earliest release to define targets SDL2::SDL2 and SDL2::SDL2main,
#   as well as define SDL_islower and SDL_isalpha for SDL_rtf 2.0.0
#   2.0.16 25f9ed87ff6947d9576fc9d79dee0784e638ac58
# - release matching Ubuntu 22.04 LTS package:
#   2.0.20 b424665e0899769b200231ba943353a5fee1b6b6
# - release matching Ubuntu 23.10 package:
#   2.28.3 8a5ba43d00252c6c8b33c9aa4f1048222955ab4d
set(FP_OPTIONS
  2.0.16
  CONFIG # looking for /usr/lib/$(gcc -dumpmachine)/cmake/SDL2/sdl2-config.cmake
  )
# CMAKE_CACHE_ARGS not passed by FetchContent to ExternalProject_Add as documented in:
#   https://cmake.org/cmake/help/v3.16/module/FetchContent.html#command:fetchcontent_declare
#   https://cmake.org/cmake/help/v3.16/module/ExternalProject.html#command:externalproject_add
# So we set the cache variable directly instead of passing with `-D`, see:
#   https://discourse.cmake.org/t/fetchcontent-cache-variables/1538
set(SDL_STATIC OFF CACHE BOOL "Toggles building of SDL2 static library")
set(FC_OPTIONS
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        8a5ba43d00252c6c8b33c9aa4f1048222955ab4d  # 2.28.3
)
fetch_if_not_found(SDL2 "${FP_OPTIONS}" "${FC_OPTIONS}")

# SDL_image not needed, as it only allocates SDL_Surface(s)

# Relevant SDL2_mixer releases:
# - release matching Ubuntu 20.04 LTS and 22.04 LTS packages: (no
#     CMakeLists.txt, required SDL2 version unknown)
#   2.0.4 da75a58c19de9fedea62724a5f7770cbbe39adf9
# - earliest stable release to use CMakeLists.txt: (requires SDL2 2.0.9)
#   2.6.0 738611693dc324001cc00f5b800e3d18fb42cb4e
# - release matching Ubuntu 23.10 package: (requires SDL2 2.0.9)
#   2.6.3 6103316427a8479e5027e41ab9948bebfc1c3c19
set(FP_OPTIONS
  2.0.4
  CONFIG # looking for /usr/lib/$(gcc -dumpmachine)/cmake/SDL2_mixer/sdl2_mixer-config.cmake
  )
set(FC_OPTIONS
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG        6103316427a8479e5027e41ab9948bebfc1c3c19 # 2.6.3
  )
fetch_if_not_found(SDL2_mixer "${FP_OPTIONS}" "${FC_OPTIONS}")

# Relevant SDL2_net releases:
# - release matching Ubuntu 20.04 LTS and 22.04 LTS packages: (no
#     CMakeLists.txt, required SDL2 version unknown)
#   2.0.1 6e513e390d18ad7950d9082863bfe33a0c62fd71
# - earliest stable release to use CMakeLists.txt, and release matching Ubuntu
#     23.10 package: (requires SDL2 2.0.9)
#   2.2.0 669e75b84632e2c6cc5c65974ec9e28052cb7a4e
set(FP_OPTIONS
  2.0.1
  CONFIG # looking for /usr/lib/$(gcc -dumpmachine)/cmake/SDL2_net/sdl2-net.cmake
  )
set(FC_OPTIONS
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_net.git
  GIT_TAG        669e75b84632e2c6cc5c65974ec9e28052cb7a4e # 2.2.0
  )
fetch_if_not_found(SDL2_net "${FP_OPTIONS}" "${FC_OPTIONS}")

# Relevant SDL2_ttf releases:
# - release matching Ubuntu 20.04 LTS package:
#   2.0.15 33cdd1881e31184b49a68b4890d1d256fc0c6dc1
# - release matching Ubuntu 22.04 LTS package:
#   2.0.18 3e702ed9bf400b0a72534f144b8bec46ee0416cb
# - release matching Ubuntu 23.10 package: (requires SDL2 2.0.10)
#   2.20.2 89d1692fd8fe91a679bb943d377bfbd709b52c23
set(FP_OPTIONS
  2.0.15
  CONFIG # looking for /usr/lib/$(gcc -dumpmachine)/cmake/SDL2_ttf/sdl2-ttf.cmake
  )
set(FC_OPTIONS
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG        89d1692fd8fe91a679bb943d377bfbd709b52c23 # 2.20.2
  )
# If SDL2_ttf is built from source by FetchContent, we need to consider its
#   CMakeLists.txt, and force it to build its dependencies from source as well.
find_package(Harfbuzz QUIET)
find_package(Freetype QUIET)
if(NOT Freetype_FOUND OR NOT Harfbuzz_FOUND)
  # override SDL_ttf 2.20.0+ option SDL2TTF_VENDORED to force building of
  #   dependency submodules, see:
  #   - https://github.com/libsdl-org/SDL_ttf/blob/7e4a456bf463b887b94c191030dd742d7654d6ff/CMakeLists.txt#L61
  set(SDL2TTF_VENDORED TRUE)
endif()
fetch_if_not_found(SDL2_ttf "${FP_OPTIONS}" "${FC_OPTIONS}")

# Relevant SDL2_rtf releases:
# - !!! Does not appear as Ubuntu 20.04 LTS, 22.04 LTS, or 23.10 package
# - current v2 release at last script update: (requires SDL2 2.0.16
#     due to use of SDL_islower (2.0.12+) and SDL_isalpha (2.0.16+))
#   2.0.0 db0e4676d6f9f6a271747ae21f997c3743cd53e1
set(FP_OPTIONS
  2.0.0
  CONFIG # looking for /usr/lib/$(gcc -dumpmachine)/cmake/SDL2_rtf/sdl2-rtf.cmake
  )
set(FC_OPTIONS
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_rtf.git
  GIT_TAG        db0e4676d6f9f6a271747ae21f997c3743cd53e1 # 2.0.0
  )
fetch_if_not_found(SDL2_rtf "" "${FC_OPTIONS}")
