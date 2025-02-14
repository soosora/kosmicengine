#pragma once
#include <cstdint>
#include <memory>

namespace Kosmic::Renderer {

class Framebuffer {
public:
    virtual ~Framebuffer() = default;
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
    virtual void Resize(uint32_t width, uint32_t height) = 0;
    
    static std::shared_ptr<Framebuffer> Create(uint32_t width, uint32_t height);
};

} // namespace Kosmic::Renderer
