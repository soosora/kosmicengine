#include "Kosmic/Core/Application.hpp"
#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Core/Math/Math.hpp"
#include "Kosmic/Renderer/Mesh.hpp"
#include "Kosmic/Core/Logging.hpp"
#include <SDL2/SDL.h>

using namespace Kosmic;

class SandboxApp : public Application {
public:
	SandboxApp() : Application("Sandbox", 800, 600) {}

private:
    Renderer::Renderer3D renderer;
    std::shared_ptr<Renderer::Camera> camera;

    // Add input handling using SDL
    bool IsKeyPressed(SDL_Keycode key) {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        return state[SDL_GetScancodeFromKey(key)];
    }

protected:
    // Initialization
	void OnInit() override {
		KOSMIC_INFO("(Sandbox) SandboxApp OnInit called.");
		// Initialize the 3D renderer
		renderer.Init();
		KOSMIC_INFO("(Sandbox) Renderer initialized.");
        
        // Setup camera instance
        camera = std::make_shared<Renderer::Camera>(45.0f, 800.0f/600.0f);
        camera->SetPosition({0.0f, 0.0f, 3.0f});
        renderer.SetCamera(camera);
		KOSMIC_INFO("(Sandbox) Camera setup complete.");
	}
	
    // Update logic
	void OnUpdate(float deltaTime) override {
        // Example: Simple camera movement
        if (IsKeyPressed(SDLK_w)) camera->SetPosition(camera->GetPosition() + Math::Vector3(0.0f, 0.0f, -0.1f));
        if (IsKeyPressed(SDLK_s)) camera->SetPosition(camera->GetPosition() + Math::Vector3(0.0f, 0.0f, 0.1f));
		if (IsKeyPressed(SDLK_a)) camera->SetPosition(camera->GetPosition() + Math::Vector3(-0.1f, 0.0f, 0.0f));
		if (IsKeyPressed(SDLK_d)) camera->SetPosition(camera->GetPosition() + Math::Vector3(0.1f, 0.0f, 0.0f));
		if (IsKeyPressed(SDLK_SPACE)) camera->SetPosition(camera->GetPosition() + Math::Vector3(0.0f, 0.1f, 0.0f));
		if (IsKeyPressed(SDLK_LSHIFT)) camera->SetPosition(camera->GetPosition() + Math::Vector3(0.0f, -0.1f, 0.0f));

		float rotationSpeed = 50.0f * deltaTime; // Degrees per second

		float currentPitch = camera->GetPitch();
		float currentYaw = camera->GetYaw();

		if (IsKeyPressed(SDLK_q))
		{
			currentYaw -= rotationSpeed;
			camera->SetRotation(currentPitch, currentYaw);
		}

		if (IsKeyPressed(SDLK_e))
		{
			currentYaw += rotationSpeed;
			camera->SetRotation(currentPitch, currentYaw);
		}
	}

    // Rendering
	void OnRender() override {
		// Example: Render cube
		renderer.SetMesh(Renderer::MeshLibrary::Cube());
		// Call render to draw the mesh
		renderer.Render();
	}

    // Cleaning
	void OnCleanup() override {}
};

int main() {
    Kosmic::Log::Init(); // Initialize logger
	KOSMIC_INFO("(Sandbox) Starting SandboxApp...");
	SandboxApp app;
	app.Run();
	KOSMIC_INFO("(Sandbox) SandboxApp terminated.");
	return 0;
}
