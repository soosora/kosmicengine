#pragma once

#include "RendererAPI.hpp"

namespace Kosmic::Renderer {

class OpenGLRendererAPI : public RendererAPI {
public:
    void Init() override;
    void SetViewport(int x, int y, int width, int height) override;
    void Clear() override;
    void DrawIndexed(uint32_t count) override;
};

RendererAPI* GetOpenGLRendererAPI();

} // namespace Kosmic::Renderer
