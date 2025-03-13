# TBD requires v3.X
# cmake_minimum_required(VERSION 3.10)

# should set Catch2_VERSION_MAJOR
include(GetCatch2)
# safeSdlCall links SDL2 core and SDL_net; remaining extensions only used in unit testing
include(GetSDL2_image)
include(GetSDL2_mixer)
include(GetSDL2_ttf)
include(GetSDL2_rtf)  # requires SDL2_ttf to be defined first

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

add_executable(safeSdlCall_tests
  safeSdlCall_test.cc
)
set_target_properties(safeSdlCall_tests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
  )
set_strict_compile_options(safeSdlCall_tests)
target_compile_definitions(safeSdlCall_tests
  PUBLIC
    _CATCH_VERSION_MAJOR=${Catch2_VERSION_MAJOR}
    # quotes are passed into macros (expecting EXAMPLE_DATA_DIR to be string literal)
    EXAMPLE_DATA_DIR="${PROJECT_SOURCE_DIR}/test/example_data/"
  )
target_link_libraries(safeSdlCall_tests
  PRIVATE
    safeSdlCall
    Catch2::Catch2WithMain
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    SDL2_rtf::SDL2_rtf
  )

# CTest.cmake, Catch.cmake, catch_discover_tests, see:
#   - https://github.com/catchorg/Catch2/blob/v3.4.0/docs/cmake-integration.md

# In order to cause a test to fail with a non-zero exit code if memcheck fails,
#   we need to pass extra options to valgrind. One way to do this is with
#   MEMORYCHECK_COMMAND_OPTIONS, used by DartConfiguration.tcl.in to set
#   MemoryCheckCommandOptions, used in turn by ctest memcheck, see:
#       - https://stackoverflow.com/a/56116311
#       - https://github.com/Kitware/CMake/blob/v3.27.4/Modules/CTest.cmake#L264
#       - https://github.com/Kitware/CMake/blob/v3.27.4/Modules/CTestTargets.cmake#L30
#       - https://github.com/Kitware/CMake/blob/v3.27.4/Modules/DartConfiguration.tcl.in
# default memcheck options (observable with ctest --verbose):
#   "-q" "--tool=memcheck" "--leak-check=yes" "--show-reachable=yes" "--num-callers=50"
# setting memcheck error exit code as high as possible, as Catch2 can return
#   an exit code of 0-255 (unlikely to be 255 failed cases in one test)
set(MEMORYCHECK_COMMAND_OPTIONS
    "-q --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=50 \
--error-exitcode=255 --gen-suppressions=all"
)
set(MEMORYCHECK_SUPPRESSIONS_FILE
  ${CMAKE_CURRENT_LIST_DIR}/SDL2.supp
)

include(CTest) # calls enable_testing(), sets up DartConfiguration.tcl
include(Catch)
catch_discover_tests(safeSdlCall_tests
  # each discovered test will be run as a separate process, and the Catch2 exit
  #   code for all tests skipped in a process is 4, see:
  #   - https://github.com/catchorg/Catch2/blob/v3.4.0/src/catch2/catch_session.cpp#L348
  PROPERTIES SKIP_RETURN_CODE 4
)

add_custom_command(TARGET safeSdlCall_tests POST_BUILD
  # cmake auto concatenates and space delimits quoted strings in this context
  COMMAND
    "ctest"
      "-C" "$<CONFIGURATION>"
      "--test-action" "memcheck"
      # If SDL libraries and their dependencies are fetched and not found,
      #   during configuration dependencies will register their own tests;
      #   --tests-regex filters for only safeSdlCall tests. Would prefer to
      #   filter by label safeSdlCall, but labels are not properly imported
      #   from Catch2 tags, see:
      #   - https://github.com/catchorg/Catch2/issues/1590
      "--tests-regex" "SDL"
      # --output-on-failure will not show output for skipped tests; to inspect
      #   errors on skipped tests use --verbose instead
      "--output-on-failure"
  # As of cmake 3.27.4, DartConfiguration.tcl placed in PROJECT_BINARY_DIR
  #  regardless of where enable_testing()/include(CTest) is called, see:
  #  - https://github.com/Kitware/CMake/blob/v3.27.4/Modules/CTestTargets.cmake#L30
  WORKING_DIRECTORY
    ${PROJECT_BINARY_DIR}
)
