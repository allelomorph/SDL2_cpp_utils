cmake_minimum_required(VERSION 3.16)

project(SDL2_cpp_utils
  DESCRIPTION "Resources for C++ projects linking SDL2"
  LANGUAGES CXX
)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(safeSdlCall)
add_subdirectory(sdl2_smart_ptrs)
