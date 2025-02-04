#pragma once

#include <memory>

namespace Kosmic::Renderer {

class Renderer3D {
public:
    Renderer3D();
    ~Renderer3D();

    void Init();
    void Render();

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace Kosmic::Renderer