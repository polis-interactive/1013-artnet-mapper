cmake_minimum_required(VERSION 3.16)

set(CMAKE_VERBOSE_MAKEFILE ON)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    message("Setting debug flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS_RELEASE_INIT "-Wall")
endif()

set(CMAKE_CXX_STANDARD 20)

SET(AM_PLATFORM "X86_LINUX" CACHE STRING "Sets up the build env")

include(cmake/platform.cmake)
include(cmake/toolchain.cmake)
include(cmake/features.cmake)

project (
    ArtnetMapper
    VERSION 0.1
    LANGUAGES C CXX
)


# post-project cmake files
include(cmake/boost.cmake)
include(cmake/graphics.cmake)

set(root_dir ${CMAKE_CURRENT_LIST_DIR})
set(internal_dir ${root_dir}/internal)
set(third_party_dir ${root_dir}/third_party)

add_definitions(-DROOT_DIR="${CMAKE_CURRENT_LIST_DIR}")
add_definitions(-DCONFIG_DIR="${root_dir}/config")
add_definitions(-DASSETS_DIR="${root_dir}/assets")
add_definitions(-DSHADER_DIR="${root_dir}/assets/shaders")
add_definitions(-DTEXTURE_DIR="${root_dir}/assets/textures")

include_directories(${root_dir}/internal)
include_directories(${third_party_dir})

add_subdirectory(${internal_dir}/infrastructure)
add_subdirectory(${internal_dir}/service)

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    add_subdirectory(${root_dir}/test)
endif()

add_subdirectory(${root_dir}/application)