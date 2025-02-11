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

add_executable(Pong src/main.cpp)

target_link_libraries(Pong PRIVATE
    KosmicEngine
)

if(WIN32)
    # Link Windows-specific OpenGL library.
    target_link_libraries(Pong PRIVATE opengl32)
endif()
