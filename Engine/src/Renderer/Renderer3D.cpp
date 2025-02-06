#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Renderer/Shader.hpp"
#include "Kosmic/Renderer/Mesh.hpp"
#include <GL/glew.h>
#include "Kosmic/Core/Logging.hpp"
#include <iostream>
#include <cstdint>

namespace Kosmic::Renderer {

// Store the last GPU time in nanoseconds
static uint64_t s_LastGPUTime = 0;

class Renderer3D::Impl {
public:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Camera> camera;
    GLuint queryID = 0;      // OpenGL query object for GPU timing
    uint64_t gpuTime = 0;    // Last measured GPU time (ns)
};

Renderer3D::Renderer3D() : pImpl(std::make_unique<Impl>()) {}
Renderer3D::~Renderer3D() {
    if (pImpl->queryID)
        glDeleteQueries(1, &pImpl->queryID);
}

void Renderer3D::Init() {
    // Initial OpenGL Settings for 3D Rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Create and configure shader
    pImpl->shader = Shader::CreateBasicShader();
    
    // Create OpenGL query object for GPU timing
    glGenQueries(1, &pImpl->queryID);
    
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
    
    // Begin GPU timing query
    glBeginQuery(GL_TIME_ELAPSED, pImpl->queryID);
    
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
    
    // End GPU timing query
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectui64v(pImpl->queryID, GL_QUERY_RESULT, &pImpl->gpuTime);
    s_LastGPUTime = pImpl->gpuTime; // Update global GPU time
}

uint64_t Renderer3D::GetLastGPUTime() {
    return s_LastGPUTime;
}

} // namespace Kosmic::Renderer
