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

add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC 
    imgui
    imgui/backends
)
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

add_library(ThirdpartyLibs INTERFACE)
target_link_libraries(ThirdpartyLibs INTERFACE
    ImGui
    stb
    EnTT
    spdlog::spdlog
)
