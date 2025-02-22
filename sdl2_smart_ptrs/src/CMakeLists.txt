cmake_minimum_required(VERSION 3.10)

include(GetSDL2)
include(GetSDL2_mixer)
include(GetSDL2_net)
include(GetSDL2_ttf)
include(GetSDL2_rtf)  # requires SDL2_ttf to be defined first

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

add_library(sdl2_smart_ptrs_obj OBJECT
  sdl2_smart_ptr.cc
  sdl2_mixer_smart_ptr.cc
  sdl2_net_smart_ptr.cc
  sdl2_rtf_smart_ptr.cc
  sdl2_ttf_smart_ptr.cc
  )
set_target_properties(sdl2_smart_ptrs_obj PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
  )
set_strict_compile_options(sdl2_smart_ptrs_obj)
target_include_directories(sdl2_smart_ptrs_obj PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )
target_link_libraries(sdl2_smart_ptrs_obj
  SDL2::SDL2
  SDL2_mixer::SDL2_mixer
  SDL2_net::SDL2_net
  SDL2_rtf::SDL2_rtf
  SDL2_ttf::SDL2_ttf
  )

add_library(sdl2_smart_ptrs_static STATIC)
target_link_libraries(sdl2_smart_ptrs_static sdl2_smart_ptrs_obj)
target_include_directories(sdl2_smart_ptrs_static INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )
set_target_properties(sdl2_smart_ptrs_static PROPERTIES
  ARCHIVE_OUTPUT_NAME sdl2_smart_ptrs
  )

add_library(sdl2_smart_ptrs_shared SHARED)
target_link_libraries(sdl2_smart_ptrs_shared sdl2_smart_ptrs_obj)
target_include_directories(sdl2_smart_ptrs_shared INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  )
set_target_properties(sdl2_smart_ptrs_shared PROPERTIES
  LIBRARY_OUTPUT_NAME sdl2_smart_ptrs
  )
