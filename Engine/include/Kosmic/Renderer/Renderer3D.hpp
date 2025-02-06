#pragma once

#include <memory>
#include "Camera.hpp"

namespace Kosmic::Renderer {

class Renderer3D {
public:
    Renderer3D();
    ~Renderer3D();

    void Init();
    void Render();
    void SetCamera(const std::shared_ptr<Camera>& camera);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace Kosmic::Renderer