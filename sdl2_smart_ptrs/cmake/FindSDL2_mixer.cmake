# TBD cmake version?

# TBD add docs
# About use of of Restructured Text/`.rst` in cmake module docs:
#   https://github.com/Kitware/CMake/blob/master/Help/dev/documentation.rst#modules

#[=======================================================================[.rst:

#]=======================================================================]

# LINUX, BSD variables not available until cmake v3.25
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  message(WARNING
    "SDL2_mixer detection not currently cross-platform for non-Linux systems")
endif()

# https://cmake.org/cmake/help/latest/command/find_package.html#package-file-interface-variables
if(NOT SDL2_FOUND AND
    NOT sdl2_POPULATED AND
    NOT SDL2_mixer_FIND_QUIETLY)
    message(WARNING
      "Could not detect SDL2, which is required by SDL2_mixer")
endif()

# Search for the SDL2_mixer include directory
find_path(SDL2_MIXER_INCLUDE_DIR
  SDL_mixer.h
  PATH_SUFFIXES
    SDL2
    include/SDL2
    include
  DOC "Path to directory containing SDL2_mixer headers"
)

if (SDL2_MIXER_INCLUDE_DIR)
  # set SDL2_MIXER_VERSION
  include(SdlVersionFromHeader)
  sdl_version_from_header(SDL_mixer.h)
endif()

# Search for the SDL2_mixer library
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()
#unset(SDL2_MIXER_LIBRARY CACHE)
find_library(SDL2_MIXER_LIBRARY
  SDL2_mixer
  PATH_SUFFIXES
    lib
    ${VC_LIB_PATH_SUFFIX}
  DOC "Path to directory containing SDL2_mixer library"
)
unset(VC_LIB_PATH_SUFFIX)

set(SDL2_MIXER_LIBRARIES ${SDL2_MIXER_LIBRARY})
set(SDL2_MIXER_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_mixer
  REQUIRED_VARS
    SDL2_MIXER_LIBRARIES
    SDL2_MIXER_INCLUDE_DIRS
  VERSION_VAR
    SDL2_MIXER_VERSION
  )

# In the case that find_package(SDL2_mixer) was called as part of FetchContent,
#   and it is determined that it will instead be built from source, we need to
#   consider SDL_mixer's CMakeLists.txt, and force it to build its dependencies
#   from source as well.
if(NOT SDL2_mixer_FOUND)
  find_package(opusfile QUIET)
  find_package(tremor QUIET)
  find_package(vorbisfile QUIET)
  find_package(FLAC QUIET)
  find_package(modplug QUIET)
  find_package(libxmp-lite QUIET)
  find_package(libxmp QUIET)
  find_package(MPG123 QUIET)
  find_package(FluidSynth QUIET)
  if (NOT opusfile_FOUND OR
      NOT tremor_FOUND OR
      NOT vorbisfile_FOUND OR
      NOT FLAC_FOUND OR
      NOT modplug_FOUND OR
      NOT libxmp-lite_FOUND OR
      NOT libxmp_FOUND OR
      NOT MPG123_FOUND OR
      NOT FluidSynth_FOUND)
    # override SDL_mixer 2.6.0+ option SDL2MIXER_VENDORED to force building of
    #   dependency submodules, see:
    #   - https://github.com/libsdl-org/SDL_mixer/blob/738611693dc324001cc00f5b800e3d18fb42cb4e/CMakeLists.txt#L63
    set(SDL2MIXER_VENDORED TRUE)
  endif()
endif()

# TBD garnish with results of pkg-config, eg -D_REENTRANT
# SDL2_mixer::SDL2_mixer matches target name if built from source with FetchContent
if(SDL2_mixer_FOUND AND
    NOT TARGET SDL2_mixer::SDL2_mixer)
  add_library(SDL2_mixer::SDL2_mixer UNKNOWN IMPORTED)
  set_target_properties(SDL2_mixer::SDL2_mixer PROPERTIES
    IMPORTED_LOCATION "${SDL2_MIXER_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_MIXER_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES SDL2::SDL2
    )
endif()

mark_as_advanced(SDL2_MIXER_LIBRARY
                 SDL2_MIXER_INCLUDE_DIR)