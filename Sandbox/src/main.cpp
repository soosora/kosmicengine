#include "Kosmic/Core/Application.hpp"
#include "Kosmic/Renderer/Renderer3D.hpp"

class SandboxApp : public Kosmic::Application {
public:
	SandboxApp() : Kosmic::Application("Sandbox", 800, 600) {}

protected:
    // Initialization
	void OnInit() override {
		// Initialize the 3D renderer
		renderer.Init();
	}
	
    // Update logic (if necessary)
	void OnUpdate(float deltaTime) override {
	}

    // Rendering
	void OnRender() override {
		// Render the example triangle
		renderer.Render();
	}

    // Cleaning
	void OnCleanup() override {}

private:
	Kosmic::Renderer::Renderer3D renderer;
};

int main() {
	SandboxApp app;
	app.Run();
	return 0;
}
