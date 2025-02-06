#include "Kosmic/Core/Application.hpp"
#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Core/Math/Math.hpp"
#include <SDL2/SDL.h>

using namespace Kosmic;

class SandboxApp : public Application {
public:
	SandboxApp() : Application("Sandbox", 800, 600) {}

private:
    Kosmic::Renderer::Renderer3D renderer;
    std::shared_ptr<Kosmic::Renderer::Camera> camera;

    // Add input handling using SDL
    bool IsKeyPressed(SDL_Keycode key) {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        return state[SDL_GetScancodeFromKey(key)];
    }

protected:
    // Initialization
	void OnInit() override {
		// Initialize the 3D renderer
		renderer.Init();
        
        // Setup camera instance
        camera = std::make_shared<Kosmic::Renderer::Camera>(45.0f, 800.0f/600.0f);
        camera->SetPosition({0.0f, 0.0f, 3.0f});
        renderer.SetCamera(camera);
	}
	
    // Update logic
	void OnUpdate(float deltaTime) override {
        // Example: Simple camera movement
        if (IsKeyPressed(SDLK_w)) camera->SetPosition(camera->GetPosition() + Math::Vector3(0.0f, 0.0f, -0.1f));
        if (IsKeyPressed(SDLK_s)) camera->SetPosition(camera->GetPosition() + Math::Vector3(0.0f, 0.0f, 0.1f));
	}

    // Rendering
	void OnRender() override {
		// Render triangle
		renderer.Render();
	}

    // Cleaning
	void OnCleanup() override {}
};

int main() {
	SandboxApp app;
	app.Run();
	return 0;
}
