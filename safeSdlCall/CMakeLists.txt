# newest features used: FetchContent v3.11, FetchContent_MakeAvailable v3.14
cmake_minimum_required(VERSION 3.14)

project(SafeSdlCall
  VERSION 1.5
  DESCRIPTION "Templated C++ wrapper for SDL2 function calls; throws failure \
    return values as exceptions"
  LANGUAGES CXX
  )

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

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

add_subdirectory(src)
