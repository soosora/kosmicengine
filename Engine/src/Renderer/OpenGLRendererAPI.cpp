#include "Kosmic/Renderer/OpenGLRendererAPI.hpp"
#include <GL/glew.h>
#include "Kosmic/Core/Logging.hpp"
#include "Kosmic/Renderer/StateManager.hpp"

namespace Kosmic::Renderer {

void OpenGLRendererAPI::Init() {
    StateManager::SetDepthTest(true);
    KOSMIC_INFO("OpenGLRendererAPI initialized.");
}

void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(uint32_t count) {
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

static OpenGLRendererAPI s_OpenGLRendererAPI;

RendererAPI* GetOpenGLRendererAPI() {
    return &s_OpenGLRendererAPI;
}

} // namespace Kosmic::Renderer
