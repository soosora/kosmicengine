#pragma once

#include <cstdint>

namespace Kosmic::Renderer {

// Enumeration for Renderer API types
enum class API { None = 0, OpenGL = 1 };

class RendererAPI {
public:
    virtual ~RendererAPI() = default;
    virtual void Init() = 0;
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    virtual void Clear() = 0;
    virtual void DrawIndexed(uint32_t count) = 0;
    
    static API GetAPI();
    static RendererAPI* Get();
};

} // namespace Kosmic::Renderer
