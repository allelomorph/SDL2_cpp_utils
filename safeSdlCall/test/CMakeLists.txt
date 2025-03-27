# TBD requires v3.X
# cmake_minimum_required(VERSION 3.10)

# safeSdlCall links SDL2 core and SDL_net; remaining extensions only used in unit testing
include(GetSDL2_image)
include(GetSDL2_mixer)
include(GetSDL2_ttf)
include(GetSDL2_rtf)  # requires SDL2_ttf to be defined first

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

if(NOT COMMAND add_catch2_tests)
  include(AddCatch2Tests)
endif()

set(tests_target unit_tests)
if(NOT PROJECT_IS_TOP_LEVEL)
  set(tests_target ${PROJECT_NAME}_${tests_target})
endif()

add_executable(${tests_target}
  safeSdlCall_test.cc
)
set_target_properties(${tests_target} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
  )
set_strict_compile_options(${tests_target})
target_compile_definitions(${tests_target}
  PUBLIC
    # quotes are passed into macros (expecting EXAMPLE_DATA_DIR to be string literal)
    EXAMPLE_DATA_DIR="${PROJECT_SOURCE_DIR}/test/example_data/"
  )
target_link_libraries(${tests_target}
  PRIVATE
    safeSdlCall
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    SDL2_rtf::SDL2_rtf
  )

add_catch2_tests(${tests_target}
  MEMCHECK
  TEST_NAME_REGEX "SDL"
)
