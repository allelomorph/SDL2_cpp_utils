# TBD cmake version?

# TBD add docs
# About use of of Restructured Text/`.rst` in cmake module docs:
#   https://github.com/Kitware/CMake/blob/master/Help/dev/documentation.rst#modules

#[=======================================================================[.rst:

#]=======================================================================]

# LINUX, BSD variables not available until cmake v3.25
if(NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  message(WARNING
    "SDL2_net detection not currently cross-platform for non-Linux systems")
endif()

# https://cmake.org/cmake/help/latest/command/find_package.html#package-file-interface-variables
if(NOT SDL2_FOUND AND
    NOT sdl2_POPULATED AND
    NOT SDL2_net_FIND_QUIETLY)
    message(WARNING
      "Could not detect SDL2, which is required by SDL2_net")
endif()

# Search for the SDL2_net include directory
find_path(SDL2_NET_INCLUDE_DIR
  SDL_net.h
  PATH_SUFFIXES
    SDL2
    include/SDL2
    include
  DOC "Path to directory containing SDL2_net headers"
)

if (SDL2_NET_INCLUDE_DIR)
  # set SDL2_NET_VERSION
  include(SdlVersionFromHeader)
  sdl_version_from_header(SDL_net.h)
endif()

# Search for the SDL2_net library
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(VC_LIB_PATH_SUFFIX lib/x64)
else()
  set(VC_LIB_PATH_SUFFIX lib/x86)
endif()
find_library(SDL2_NET_LIBRARY
  SDL2_net
  PATH_SUFFIXES
    lib
    ${VC_LIB_PATH_SUFFIX}
  DOC "Path to directory containing SDL2_net library"
)
unset(VC_LIB_PATH_SUFFIX)

set(SDL2_NET_LIBRARIES ${SDL2_NET_LIBRARY})
set(SDL2_NET_INCLUDE_DIRS ${SDL2_NET_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2_net
  REQUIRED_VARS
    SDL2_NET_LIBRARIES
    SDL2_NET_INCLUDE_DIRS
  VERSION_VAR
    SDL2_NET_VERSION
  )

# In the case that find_package(SDL2_net) was called as part of FetchContent,
#   and it is determined that it will instead be built from source, we need to
#   consider SDL_net's CMakeLists.txt, and force it to build its dependencies
#   from source as well.
if(NOT SDL2_net_FOUND)
  # SDL_net has no apparent vendored dependencies, and no option
  #   SDL2NET_VENDORED to override, see:
  #   - https://github.com/libsdl-org/SDL_net/blob/33531ee02f59640056947f8e4f0ee1a805916ca6/CMakeLists.txt
  # set(SDL2NET_VENDORED TRUE)
endif()

# TBD garnish with results of pkg-config, eg -D_REENTRANT
# SDL2_net::SDL2_net matches target name if built from source with FetchContent
if(SDL2_net_FOUND AND
    NOT TARGET SDL2_net::SDL2_net)
  add_library(SDL2_net::SDL2_net UNKNOWN IMPORTED)
  set_target_properties(SDL2_net::SDL2_net PROPERTIES
    IMPORTED_LOCATION "${SDL2_NET_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_NET_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES SDL2::SDL2
    )
endif()

mark_as_advanced(SDL2_NET_LIBRARY
                 SDL2_NET_INCLUDE_DIR)
