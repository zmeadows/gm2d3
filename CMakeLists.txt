cmake_minimum_required (VERSION 2.8)
project (GM2D3)
# The version number.
set (GM2D3_VERSION_MAJOR 0)
set (GM2D3_VERSION_MINOR 1)

# files from GM2D3
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

# add the executable
add_executable(gm2d3 ${SOURCES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package (FLTK)
if (FLTK_FOUND)
  include_directories(${FLTK_INCLUDE_DIRS})
  target_link_libraries (gm2d3 ${FLTK_LIBRARIES})
endif (FLTK_FOUND)

find_package (Config++ REQUIRED)
if (CONFIG++_FOUND)
  include_directories(${CONFIG++_INCLUDE_DIRS})
  target_link_libraries (gm2d3 ${CONFIG++_LIBRARIES})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lconfig++")
endif (CONFIG++_FOUND)

configure_file( cmake/gm2d3_version.h.in ${CMAKE_SOURCE_DIR}/include/gm2d3_version.h )
include_directories( ${CMAKE_BINARY_DIR}/generated/ )

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
