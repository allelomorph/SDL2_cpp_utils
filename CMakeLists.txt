cmake_minimum_required(VERSION 3.16)

project(SDL2_cpp_utils
  DESCRIPTION "Resources for C++ projects linking SDL2"
  LANGUAGES CXX
)

# test for population first in case of use in parent project
if(NOT cmake_utils_POPULATED)
  include(FetchContent)
  FetchContent_Declare(cmake_utils
    GIT_REPOSITORY https://github.com/allelomorph/cmake_utils.git
    # ExternalProject_Add defaults to origin/master up to at least cmake 3.30, see:
    #   - https://cmake.org/cmake/help/v3.30/module/ExternalProject.html#git
    GIT_TAG        254d8fa33aad7947b09cd3ae5e951f89e327c20f  # origin/main
  )
  FetchContent_MakeAvailable(cmake_utils)
  list(APPEND CMAKE_MODULE_PATH ${cmake_utils_SOURCE_DIR})
endif()

include(PreventInSourceBuild)

add_subdirectory(safeSdlCall)
add_subdirectory(sdl2_smart_ptrs)
