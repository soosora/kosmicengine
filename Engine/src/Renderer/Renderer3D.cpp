#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Renderer/Shader.hpp"
#include "Kosmic/Renderer/Mesh.hpp"
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
    // Procedural sky
    std::shared_ptr<Shader> skyShader;
    std::shared_ptr<Mesh> skyMesh;
};

Renderer3D::Renderer3D() : pImpl(std::make_unique<Impl>()) {}
Renderer3D::~Renderer3D() {
    if (pImpl->queryID)
        glDeleteQueries(1, &pImpl->queryID);
}

void Renderer3D::Init() {
    // Initial OpenGL Settings for 3D Rendering
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LESS);    // Depth testing function
    glEnable(GL_CULL_FACE);  // Enable face culling
    glCullFace(GL_BACK);     // Cull back faces
    glFrontFace(GL_CCW);     // Counter-clockwise winding order

    // Create and configure shader
    pImpl->shader = Shader::CreateBasicShader();
    
    // Create sky shader and sky mesh (a cube used for sky dome)
    pImpl->skyShader = Shader::CreateSkyShader();
    pImpl->skyMesh   = MeshLibrary::Cube();
    
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

void Renderer3D::RenderSky() {
    // Disable depth writing
    glDepthMask(GL_FALSE);
    // Set face culling to render inside of the cube
    glCullFace(GL_FRONT);
    
    pImpl->skyShader->Bind();
    pImpl->skyShader->SetMat4("view", pImpl->camera->GetViewMatrixNoTranslation());
    pImpl->skyShader->SetMat4("projection", pImpl->camera->GetProjectionMatrix());
    
    pImpl->skyMesh->Draw();
    
    pImpl->skyShader->Unbind();
    
    // Restore default culling order
    glCullFace(GL_BACK);
    // Re-enable depth writing
    glDepthMask(GL_TRUE);
}

void Renderer3D::Render() {
    // Begin GPU timing query
    glBeginQuery(GL_TIME_ELAPSED, pImpl->queryID);
    
    // Clear buffers at start of frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render procedural sky first (does not affect scene depth)
    RenderSky();
    
    pImpl->shader->Bind();
    // Set texture uniform
    pImpl->shader->SetInt("u_Texture", 0);
    pImpl->shader->SetMat4("view", pImpl->camera->GetViewMatrix());
    pImpl->shader->SetMat4("projection", pImpl->camera->GetProjectionMatrix());
    
    if(pImpl->mesh) { // Render provided mesh
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

std::shared_ptr<Shader> Renderer3D::GetShader() {
    return pImpl->shader;
}

} // namespace Kosmic::Renderer
