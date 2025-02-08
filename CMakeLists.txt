cmake_minimum_required(VERSION 3.25)

project(KosmicEngine 
    VERSION 0.3.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type: Debug or Release" FORCE)
endif()

set(allowed_build_types Debug Release)
list(FIND allowed_build_types ${CMAKE_BUILD_TYPE} BUILD_TYPE_INDEX)
if(BUILD_TYPE_INDEX EQUAL -1)
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}. Choose between Debug or Release.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING "Launch build flags" FORCE)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# Copy the `Resources` directory to the Build directory
file(COPY ${CMAKE_SOURCE_DIR}/Resources
    DESTINATION ${CMAKE_BINARY_DIR})

# ----- Windows-specific configuration -----
if(WIN32)
    message(STATUS "Compiling for Windows platform")
    add_compile_definitions(PLATFORM_WINDOWS)
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -pedantic)
    endif()
endif()

add_subdirectory(Engine)
add_subdirectory(Thirdparty)
add_subdirectory(Sandbox)
