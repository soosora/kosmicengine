# Use ccache to speed up compilation
set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

add_executable(Sandbox src/main.cpp)

target_link_libraries(Sandbox PRIVATE
    KosmicEngine
)

if(WIN32)
    # Link Windows-specific OpenGL library.
    target_link_libraries(Sandbox PRIVATE opengl32)
endif()
