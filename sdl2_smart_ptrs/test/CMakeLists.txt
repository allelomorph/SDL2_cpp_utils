# TBD requires v3.X
# cmake_minimum_required(VERSION 3.10)

# should set _CATCH_VERSION_MAJOR
include(GetCatch2)

if(NOT COMMAND set_strict_compile_options)
  include(SetStrictCompileOptions)
endif()

add_executable(unit_tests
  sdl2_mixer_smart_ptr_test.cc
  sdl2_net_smart_ptr_test.cc
  sdl2_rtf_smart_ptr_test.cc
  sdl2_smart_ptr_test.cc
  sdl2_ttf_smart_ptr_test.cc
)
set_target_properties(unit_tests PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  POSITION_INDEPENDENT_CODE ON
  )
set_strict_compile_options(unit_tests)
target_compile_definitions(unit_tests
  PUBLIC
    _CATCH_VERSION_MAJOR=${_CATCH_VERSION_MAJOR}
    # quotes are passed into macros (expecting EXAMPLE_DATA_DIR to be string literal)
    EXAMPLE_DATA_DIR="${PROJECT_SOURCE_DIR}/test/example_data/"
  )
target_link_libraries(unit_tests
  PRIVATE
    sdl2_smart_ptrs_shared
    Catch2::Catch2WithMain
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
--error-exitcode=255"
)

include(CTest) # calls enable_testing(), sets up DartConfiguration.tcl
include(Catch)
catch_discover_tests(unit_tests
  # each discovered test will be run as a separate process, and the Catch2 exit
  #   code for all tests skipped in a process is 4, see:
  #   - https://github.com/catchorg/Catch2/blob/v3.4.0/src/catch2/catch_session.cpp#L348
  PROPERTIES SKIP_RETURN_CODE 4
)

add_custom_command(TARGET unit_tests POST_BUILD
  # cmake auto concatenates and space delimits quoted strings in this context
  COMMAND
    "ctest"
      "-C" "$<CONFIGURATION>"
      "--test-action" "memcheck"
      # If SDL libraries and their dependencies are fetched and not found,
      #   during configuration dependencies will register their own tests;
      #   --tests-regex filters for only sdl2_smart_ptr tests. Would prefer to
      #   filter by label sdl2_smart_ptr, but labels are not properly imported
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
