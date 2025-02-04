#include "Kosmic/Core/Application.hpp"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

namespace Kosmic {

Application::Application(const std::string& title, int width, int height)
    : m_Running(false), m_Window(nullptr), m_LastFrameTime(0) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return;
    }

    // OpenGL 3.3 Core Profile Configuration
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create window
    m_Window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    if (!m_Window) {
        std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
        return;
    }

    // Cria contexto OpenGL
    m_GLContext = SDL_GL_CreateContext(m_Window);
    if (!m_GLContext) {
        std::cerr << "Error creating OpenGL context: " << SDL_GetError() << std::endl;
        return;
    }

    // Inicializa GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        return;
    }

    SDL_GL_SetSwapInterval(1); // VSync
}

Application::~Application() {
    if (m_GLContext) SDL_GL_DeleteContext(m_GLContext);
    if (m_Window) SDL_DestroyWindow(m_Window);
    SDL_Quit();
}

void Application::Run() {
    m_Running = true;
    OnInit();

    while (m_Running) {
        // Calculate delta time
        uint32_t currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - m_LastFrameTime) / 1000.0f;
        m_LastFrameTime = currentTime;

        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                m_Running = false;
            }
        }

        // Update and render
        OnUpdate(deltaTime);
        OnRender();

        SDL_GL_SwapWindow(m_Window);
    }

    OnCleanup();
}

} // namespace Kosmic
