# TBD requires v3.X
# cmake_minimum_required(VERSION 3.10)

# should set _CATCH_VERSION_MAJOR
include(GetCatch2)
# safeSdlCall links SDL2 core and SDL_net; remaining extensions only used in unit testing
include(GetSDL2_image)
include(GetSDL2_mixer)
include(GetSDL2_ttf)
include(GetSDL2_rtf)  # requires SDL2_ttf to be defined first

add_executable(unit_tests
  safeSdlCall_test.cc
)
target_link_libraries(unit_tests
  PRIVATE
    safeSdlCall
    Catch2::Catch2WithMain
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    SDL2_rtf::SDL2_rtf
  )
target_compile_definitions(unit_tests
  PUBLIC
    _CATCH_VERSION_MAJOR=${_CATCH_VERSION_MAJOR}
    # quotes are passed into macros (expecting EXAMPLE_DATA_DIR to be string literal)
    EXAMPLE_DATA_DIR="${PROJECT_SOURCE_DIR}/test/example_data/"
  )

# https://github.com/catchorg/Catch2/blob/v3.4.0/docs/cmake-integration.md
# CTest calls enable_testing()
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)

add_custom_command(TARGET unit_tests POST_BUILD
  # If SDL libraries and their dependencies are fetched and not found,
  #   during configuration dependencies will register their own tests by calling
  #   `include(CTest)`; --tests-regex filters for only safeSdlCall tests.
  #   Would prefer to filter by label safeSdlCall, but labels are not properly
  #   imported from Catch2 tags, see: https://github.com/catchorg/Catch2/issues/1590
  # safeSdlCall tests do produce stderr output, but --output-on-failure is
  #   redundant when using --verbose
  COMMAND
    ctest -C $<CONFIGURATION> --test-action memcheck --verbose --tests-regex SDL
  # As of cmake 3.27.4, DartConfiguration.tcl placed in CMAKE_BINARY_DIR
  #  regardless of where enable_testing()/include(CTest) is called, see:
  #  https://github.com/Kitware/CMake/blob/c3977582b77f8b5a4ccacf460a262ff0a75d3cc5/Modules/CTestTargets.cmake#L30
  WORKING_DIRECTORY
    ${CMAKE_BINARY_DIR}
)
