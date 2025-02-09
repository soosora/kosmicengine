# Use ccache to speed up compilation
set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

add_executable(Pong src/main.cpp)

target_link_libraries(Pong PRIVATE
    KosmicEngine
)

if(WIN32)
    # Link Windows-specific OpenGL library.
    target_link_libraries(Pong PRIVATE opengl32)
endif()
