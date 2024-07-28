# newest features used: FetchContent v3.11, FetchContent_MakeAvailable v3.14
cmake_minimum_required(VERSION 3.14)

project(SDL2_cpp_utils
  DESCRIPTION "Resources for C++ projects linking SDL2"
  LANGUAGES CXX
)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(Getcmake_utils)

include(PreventInSourceBuild)

add_subdirectory(safeSdlCall)
add_subdirectory(sdl2_smart_ptrs)
