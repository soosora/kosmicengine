#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Renderer/Shader.hpp"
#include "Kosmic/Renderer/Mesh.hpp"
#include <GL/glew.h>
#include "Kosmic/Core/Logging.hpp"
#include <iostream>

namespace Kosmic::Renderer {

class Renderer3D::Impl {
public:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Camera> camera;
};

Renderer3D::Renderer3D() : pImpl(std::make_unique<Impl>()) {}
Renderer3D::~Renderer3D() = default;

void Renderer3D::Init() {
    // Initial OpenGL Settings for 3D Rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create and configure shader
    pImpl->shader = Shader::CreateBasicShader();
    
    
    // Add error checking after OpenGL operations
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        KOSMIC_ERROR("OpenGL error during Renderer initialization: {}", err);
    }
    KOSMIC_INFO("Renderer initialization completed.");
}

void Renderer3D::SetMesh(const std::shared_ptr<Mesh>& mesh) {
    pImpl->mesh = mesh;
}

void Renderer3D::SetCamera(const std::shared_ptr<Camera>& cam) {
    pImpl->camera = cam;
}

void Renderer3D::Render() {
    // Clears the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use shader and render
    pImpl->shader->Bind();
    pImpl->shader->SetMat4("view", pImpl->camera->GetViewMatrix());
    pImpl->shader->SetMat4("projection", pImpl->camera->GetProjectionMatrix());
    
    if(pImpl->mesh) { // Render provided mesh
        // Send model transform from Mesh to shader
        pImpl->shader->SetMat4("model", pImpl->mesh->GetTransform());
        pImpl->mesh->Draw();
    }
    
    pImpl->shader->Unbind();
}

} // namespace Kosmic::Renderer
