# newest features used: FetchContent v3.11, FetchContent_MakeAvailable v3.14
cmake_minimum_required(VERSION 3.14)

project(sdl2_smart_ptrs
  DESCRIPTION "C++ smart pointer wrappers for structs allocated by SDL2 and \
its auxillary libraries"
  LANGUAGES CXX
  )

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(Getcmake_utils)

include(PreventInSourceBuild)

if(NOT COMMAND init_ctest)
  include(InitCTest)
endif()
init_ctest(
  MEMCHECK
  MEMCHECK_FAILS_TEST
  MEMCHECK_GENERATES_SUPPRESSIONS
  MEMCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/test/SDL2.supp"
)

add_subdirectory(src)
add_subdirectory(test)
