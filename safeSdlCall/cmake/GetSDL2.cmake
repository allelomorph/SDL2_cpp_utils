# newest features used: FetchContent v3.11, FetchContent_MakeAvailable v3.14,
#   CMAKE_FIND_PACKAGE_PREFER_CONFIG v3.15
cmake_minimum_required(VERSION 3.15)

include(FetchIfNotFound)

# cmake-supplied FindSDL*.cmake modules (through v3.30 at least) are written
#   for SDL1.x, and SDL2 has switched to preferring config mode, see:
#   - https://wiki.libsdl.org/SDL2/README/cmake
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
