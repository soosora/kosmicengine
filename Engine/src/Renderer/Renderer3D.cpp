#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Renderer/Shader.hpp"
#include <GL/glew.h>
#include <iostream>

// Triangle data
namespace {
GLuint g_TriangleVAO = 0;
GLuint g_TriangleVBO = 0;

// Vertices with positions and colors
float triangleVertices[] = {
    // Positions // Colors
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Red
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Green
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // Blue
};
} // namespace

namespace Kosmic::Renderer {

class Renderer3D::Impl {
public:
    std::shared_ptr<Shader> shader;
};

Renderer3D::Renderer3D() : pImpl(std::make_unique<Impl>()) {}
Renderer3D::~Renderer3D() = default;

void Renderer3D::Init() {
    // Initial OpenGL Settings for 3D Rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create and configure shader
    pImpl->shader = Shader::CreateBasicShader();

    // Configure VAO/VBO
    glGenVertexArrays(1, &g_TriangleVAO);
    glGenBuffers(1, &g_TriangleVBO);
    
    glBindVertexArray(g_TriangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, g_TriangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Add error checking after OpenGL operations
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error during Renderer3D initialization: " << err << std::endl;
    }
}

void Renderer3D::Render() {
    // Clears the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use shader and render
    pImpl->shader->Bind();
    glBindVertexArray(g_TriangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    pImpl->shader->Unbind();
}

} // namespace Kosmic::Renderer
