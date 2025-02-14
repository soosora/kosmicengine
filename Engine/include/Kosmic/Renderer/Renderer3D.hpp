#pragma once

#include <memory>
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "RenderGraph.hpp"
#include "Framebuffer.hpp"
#include "RendererAPI.hpp"
#include <GL/glew.h>

namespace Kosmic::Renderer {

class Renderer3D {
public:
    Renderer3D();
    ~Renderer3D();

    void Init();
    void Render();
    void RenderSky();
    void SetCamera(const std::shared_ptr<Camera>& camera);
    void SetMesh(const std::shared_ptr<Mesh>& mesh);
    std::shared_ptr<Shader> GetShader();
    static uint64_t GetLastGPUTime();

    void SetFramebuffer(const std::shared_ptr<Framebuffer>& framebuffer);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    std::shared_ptr<RenderGraph> m_RenderGraph;
    std::shared_ptr<Camera> m_Camera;
    std::shared_ptr<Framebuffer> m_Framebuffer;
};

} // namespace Kosmic::Renderer
