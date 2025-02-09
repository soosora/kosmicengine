# Propagate build type to dependencies
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
    set(ASSIMP_BUILD_DEBUG ON)
else()
    add_compile_definitions(NDEBUG)
    set(ASSIMP_BUILD_DEBUG OFF)
endif()

# ImGui
set(IMGUI_SOURCES
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_sdl2.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# ImGui configuration with proper Debug/Release settings
add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC
    imgui
    imgui/backends
)

# Explicit configuration for ImGui
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(ImGui PRIVATE
        IMGUI_DEBUG
        _DEBUG
    )
    target_compile_options(ImGui PRIVATE -g)
else()
    target_compile_definitions(ImGui PRIVATE NDEBUG)
    target_compile_options(ImGui PRIVATE -O3)
endif()

target_link_libraries(ImGui PUBLIC SDL2::SDL2 OpenGL::GL)

# stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)

# EnTT
add_library(EnTT INTERFACE)
target_include_directories(EnTT INTERFACE entt/single_include)

# spdlog
add_subdirectory(spdlog)
set(SPDLOG_BUILD_EXAMPLES OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_BENCH OFF)
set(SPDLOG_BUILD_DEBUG ${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# assimp
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DEBUG ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
add_subdirectory(assimp)
if(TARGET assimp)
    target_compile_options(assimp PRIVATE -Wall)
endif()

add_library(ThirdpartyLibs INTERFACE)
target_link_libraries(ThirdpartyLibs INTERFACE
    ImGui
    stb
    EnTT
    spdlog::spdlog
    assimp
)
