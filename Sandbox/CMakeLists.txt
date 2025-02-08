add_executable(Sandbox src/main.cpp)

target_link_libraries(Sandbox PRIVATE
    KosmicEngine
)

if(WIN32)
    # Link Windows-specific OpenGL library.
    target_link_libraries(Sandbox PRIVATE opengl32)
endif()
