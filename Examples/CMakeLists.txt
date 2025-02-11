cmake_minimum_required(VERSION 3.25)

if(USE_CCACHE)
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
        set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
    else()
        message(WARNING "ccache enabled, but not found. Continuing without ccache.")
    endif()
endif()

project(KosmicEngineExamples)

add_subdirectory(Sandbox)
add_subdirectory(Pong)
