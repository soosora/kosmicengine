find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_library(KosmicEngine
    src/Core/Application.cpp
    src/Core/Input.cpp
    src/Renderer/Shader.cpp
    src/Renderer/Renderer3D.cpp
    src/Renderer/Mesh.cpp
    src/Renderer/Camera.cpp
    src/Renderer/Texture.cpp
    src/Assets/Model.cpp
    src/Assets/Material.cpp
)

target_include_directories(KosmicEngine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(KosmicEngine PUBLIC
    SDL2::SDL2
    OpenGL::GL
    GLEW::GLEW
    EnTT
    glm::glm
    ImGui
    assimp
)

if(WIN32)
    # Link Windows OpenGL library explicitly
    target_link_libraries(KosmicEngine PRIVATE opengl32)
endif()
