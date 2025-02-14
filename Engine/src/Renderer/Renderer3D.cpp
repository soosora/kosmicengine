#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Renderer/Shader.hpp"
#include "Kosmic/Renderer/Mesh.hpp"
#include "Kosmic/Renderer/StateManager.hpp"
#include "Kosmic/Core/Logging.hpp"
#include "Kosmic/Renderer/Framebuffer.hpp"
#include "Kosmic/Renderer/RendererAPI.hpp"
#include "Kosmic/Renderer/OpenGLRendererAPI.hpp"
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

Renderer3D::Renderer3D() : pImpl(std::make_unique<Impl>()) {
    m_RenderGraph = std::make_shared<RenderGraph>();
}

Renderer3D::~Renderer3D() {
    if (pImpl->queryID)
        glDeleteQueries(1, &pImpl->queryID);
}

void Renderer3D::Init() {
    // Initialize the RendererAPI using OpenGL
    GetOpenGLRendererAPI()->Init();
    
    // Create and configure shader
    pImpl->shader = Shader::CreateBasicShader();
    pImpl->shader->Bind();
    
    // Set default lighting parameters
    pImpl->shader->SetVec3("u_AmbientLightColor", Math::Vector3(1.0f));
    pImpl->shader->SetFloat("u_AmbientLightIntensity", 0.7f);
    pImpl->shader->SetVec3("u_DirLightDirection", Math::Vector3(-0.2f, -1.0f, -0.3f));
    pImpl->shader->SetVec3("u_DirLightColor", Math::Vector3(1.0f));
    pImpl->shader->SetFloat("u_DirLightIntensity", 0.3f);
    
    pImpl->shader->Unbind();
    
    // Create sky shader and sky mesh (a sphere used for sky dome)
    pImpl->skyShader = Shader::CreateSkyShader();
    pImpl->skyMesh   = MeshLibrary::Sphere();
    
    // Create OpenGL query object for GPU timing
    glGenQueries(1, &pImpl->queryID);
    
    // Set default clear color to dark gray
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    // Add error checking after OpenGL operations
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        KOSMIC_ERROR("OpenGL error during Renderer initialization: {}", err);
    }
    KOSMIC_INFO("Renderer initialization completed.");

    // Initialize StateManager defaults:
    StateManager::SetCullFace(true);
    StateManager::SetDepthTest(true);
}

void Renderer3D::SetMesh(const std::shared_ptr<Mesh>& mesh) {
    pImpl->mesh = mesh;
}

void Renderer3D::SetCamera(const std::shared_ptr<Camera>& camera) {
    m_Camera = camera;
    pImpl->camera = camera;
}

void Renderer3D::SetFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer) {
    m_Framebuffer = framebuffer;
}

void Renderer3D::RenderSky() {
    // Disable depth writing
    glDepthMask(GL_FALSE);
    // Set face culling to render inside of the cube
    glCullFace(GL_FRONT);
    
    pImpl->skyShader->Bind();
    pImpl->skyShader->SetMat4("view", pImpl->camera->GetViewMatrixNoTranslation());
    pImpl->skyShader->SetMat4("projection", pImpl->camera->GetSkyboxProjectionMatrix());
    
    pImpl->skyMesh->Draw();
    
    pImpl->skyShader->Unbind();
    
    // Restore default culling order
    glCullFace(GL_BACK);
    // Re-enable depth writing
    glDepthMask(GL_TRUE);
}

void Renderer3D::Render() {
    // If a custom framebuffer is set, bind it before rendering
    if(m_Framebuffer)
        m_Framebuffer->Bind();
    
    // Begin GPU timing query
    glBeginQuery(GL_TIME_ELAPSED, pImpl->queryID);
    
    // Clear buffers using RendererAPI
    GetOpenGLRendererAPI()->Clear();
    
    // Render procedural sky first
    RenderSky();
    
    pImpl->shader->Bind();
    
    // Set default white color for objects without texture
    pImpl->shader->SetVec4("u_Color", Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Set texture uniform
    pImpl->shader->SetInt("u_Texture", 0);
    pImpl->shader->SetMat4("view", pImpl->camera->GetViewMatrix());
    pImpl->shader->SetMat4("projection", pImpl->camera->GetProjectionMatrix());
    
    if(pImpl->mesh) { // Render provided mesh
        pImpl->shader->SetMat4("model", pImpl->mesh->GetTransform());
        pImpl->mesh->Draw();
    }
    
    pImpl->shader->Unbind();
    
    m_RenderGraph->Execute();

    // End GPU timing query
    glEndQuery(GL_TIME_ELAPSED);
    glGetQueryObjectui64v(pImpl->queryID, GL_QUERY_RESULT, &pImpl->gpuTime);
    s_LastGPUTime = pImpl->gpuTime; // Update global GPU time
    
    // If a custom framebuffer was bound, unbind to render to default framebuffer
    if(m_Framebuffer)
        m_Framebuffer->Unbind();
}

uint64_t Renderer3D::GetLastGPUTime() {
    return s_LastGPUTime;
}

std::shared_ptr<Shader> Renderer3D::GetShader() {
    return pImpl->shader;
}

} // namespace Kosmic::Renderer
