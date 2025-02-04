#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Renderer/Shader.hpp"
#include "Kosmic/Renderer/Mesh.hpp"
#include <GL/glew.h>
#include <iostream>

namespace Kosmic::Renderer {

class Renderer3D::Impl {
public:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> triangle;
};

Renderer3D::Renderer3D() : pImpl(std::make_unique<Impl>()) {}
Renderer3D::~Renderer3D() = default;

void Renderer3D::Init() {
    // Initial OpenGL Settings for 3D Rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create and configure shader
    pImpl->shader = Shader::CreateBasicShader();
    
    // Create triangle mesh
    pImpl->triangle = Mesh::CreateTriangle();

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
    pImpl->triangle->Draw();
    pImpl->shader->Unbind();
}

} // namespace Kosmic::Renderer
