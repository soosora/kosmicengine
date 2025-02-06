#include "Kosmic/Core/Application.hpp"
#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Core/Math/Math.hpp"
#include "Kosmic/Renderer/Mesh.hpp"
#include "Kosmic/Renderer/Texture.hpp"
#include "Kosmic/Core/Logging.hpp"
#include "Kosmic/Renderer/Lighting.hpp"
#include "Kosmic/Core/Input.hpp"

using namespace Kosmic;

class SandboxApp : public Application {
public:
	SandboxApp() : Application("Sandbox", 800, 600) {}

private:
    Renderer::Renderer3D renderer;
    std::shared_ptr<Renderer::Camera> camera;
    std::shared_ptr<Renderer::Texture> texture;
    
    Renderer::Lighting::AmbientLight ambientLight;
    Renderer::Lighting::DirectionalLight dirLight;

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

		// Load texture
		texture = std::make_shared<Renderer::Texture>("resources/texture.png");
		KOSMIC_INFO("(Sandbox) Texture loaded.");
        
        // Initialize lighting (in white for ambient and directional)
        ambientLight.color = {1.0f, 1.0f, 1.0f};
        ambientLight.intensity = 0.3f;
        dirLight.direction = { -0.2f, -1.0f, -0.3f };
        dirLight.color = {1.0f, 1.0f, 1.0f};
        dirLight.intensity = 0.7f;
	}
	
    // Update logic
	void OnUpdate(float deltaTime) override {
        float speed = 0.1f;
        // Retrieve current position and orientation
        auto pos     = camera->GetPosition();
        auto forward = camera->GetFront();
        auto up      = Math::Vector3(0.0f, 1.0f, 0.0f);
        auto right   = Math::Normalize(Math::Cross(forward, up));
        
        // Apply movement relative to camera orientation
        if (Input::IsKeyPressed(SDLK_w)) pos = pos + forward * speed;
        if (Input::IsKeyPressed(SDLK_s)) pos = pos - forward * speed;
        if (Input::IsKeyPressed(SDLK_d)) pos = pos + right   * speed;
        if (Input::IsKeyPressed(SDLK_a)) pos = pos - right   * speed;
        if (Input::IsKeyPressed(SDLK_SPACE)) pos = pos + up   * speed;
        if (Input::IsKeyPressed(SDLK_LSHIFT)) pos = pos - up  * speed;
        
        camera->SetPosition(pos);
        
        float currentPitch = camera->GetPitch();
        float currentYaw   = camera->GetYaw();
        
        int mouseDeltaX, mouseDeltaY;
        Kosmic::Input::GetMouseDelta(mouseDeltaX, mouseDeltaY);
        if (mouseDeltaX || mouseDeltaY) {
            float sensitivity = 0.1f;
            currentYaw   += mouseDeltaX * sensitivity;
            currentPitch -= mouseDeltaY * sensitivity;
            camera->SetRotation(currentPitch, currentYaw);
        }
    }

    // Rendering
	void OnRender() override {
		// Bind texture to slot 0
		texture->Bind(0);
        
        // Configure lighting uniforms via shader obtained from renderer
        {
            auto shader = renderer.GetShader();
            shader->Bind();
            shader->SetVec3("u_AmbientLightColor", ambientLight.color);
            shader->SetFloat("u_AmbientLightIntensity", ambientLight.intensity);
            shader->SetVec3("u_DirLightDirection", dirLight.direction);
            shader->SetVec3("u_DirLightColor", dirLight.color);
            shader->SetFloat("u_DirLightIntensity", dirLight.intensity);
            shader->Unbind();
        }
        
		// Example: Render cube
		renderer.SetMesh(Renderer::MeshLibrary::Cube());
		// Render procedural sky
		renderer.Render();
		// Unbind texture
		texture->Unbind();
	}

    // Cleaning
	void OnCleanup() override {}
};

int main() {
    Log::Init(); // Initialize logger
	KOSMIC_INFO("(Sandbox) Starting SandboxApp...");
	SandboxApp app;
	app.Run();
	KOSMIC_INFO("(Sandbox) SandboxApp terminated.");
	return 0;
}
