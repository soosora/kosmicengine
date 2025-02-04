#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include <string>

namespace Kosmic {

class Application {
public:
    Application(const std::string& title = "Kosmic Engine", int width = 800, int height = 600);
    virtual ~Application();

    void Run();
    
protected:
    virtual void OnInit() = 0;
    virtual void OnUpdate(float deltaTime) = 0;
    virtual void OnRender() = 0;
    virtual void OnCleanup() = 0;

private:
    bool m_Running;
    SDL_Window* m_Window;
    SDL_GLContext m_GLContext;
    uint32_t m_LastFrameTime;
};

} // namespace Kosmic