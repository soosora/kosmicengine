cmake_minimum_required(VERSION 3.25)

project(KosmicEngine 
    VERSION 0.1.0 
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(Engine)
add_subdirectory(Thirdparty)
add_subdirectory(Sandbox)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(KE_DEBUG)
endif()
