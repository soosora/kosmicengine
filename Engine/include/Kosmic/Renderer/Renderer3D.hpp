#pragma once

#include <memory>
#include "Camera.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
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

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace Kosmic::Renderer
