cmake_minimum_required(VERSION 3.25)

project(KosmicEngine 
    VERSION 0.4.0
    LANGUAGES CXX
)

# Enable ccache usage option
option(USE_CCACHE "Enable ccache usage" ON)

# Use ccache to speed up compilation if enabled
if(USE_CCACHE)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    else()
        message(WARNING "ccache enabled, but not found. Continuing without ccache.")
    endif()
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Define build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type: Debug or Release" FORCE)
endif()

# Validate build type
set(allowed_build_types Debug Release)
list(FIND allowed_build_types ${CMAKE_BUILD_TYPE} BUILD_TYPE_INDEX)
if(BUILD_TYPE_INDEX EQUAL -1)
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}. Choose between Debug or Release.")
endif()

# Set common build flags and propagate to dependencies
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0" CACHE STRING "Debug flags" FORCE)
    add_compile_definitions(KOSMIC_DEBUG)
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3" CACHE STRING "Release flags" FORCE)
endif()

# Required packages
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Copy resources
file(COPY ${CMAKE_SOURCE_DIR}/Resources
    DESTINATION ${CMAKE_BINARY_DIR})

# Platform specific settings
if(WIN32)
    message(STATUS "Compiling for Windows platform")
    add_compile_definitions(PLATFORM_WINDOWS)
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -pedantic)
    endif()
endif()

# Add subdirectories (in specific order for dependency resolution)
add_subdirectory(Thirdparty)
add_subdirectory(Engine)
add_subdirectory(Examples)
