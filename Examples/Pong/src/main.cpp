#include "Kosmic/Core/Application.hpp"
#include "Kosmic/Renderer/Renderer3D.hpp"
#include "Kosmic/Renderer/Mesh.hpp"
#include "Kosmic/Renderer/Texture.hpp"
#include "Kosmic/Core/Logging.hpp"
#include "Kosmic/Core/Input.hpp"
#include "Kosmic/Core/Math/Math.hpp"

using namespace Kosmic;
using namespace Kosmic::Renderer;
using namespace Kosmic::Math;

// PongApp: Implements a classic Pong game using Kosmic Engine
class PongApp : public Application {
public:
    PongApp() : Application("Pong", 800, 600) {}

private:
    Renderer3D renderer;
    std::shared_ptr<Camera> camera;
    
    // Meshes for paddles and ball
    std::shared_ptr<Mesh> leftPaddle;
    std::shared_ptr<Mesh> rightPaddle;
    std::shared_ptr<Mesh> ball;

    // Game parameters (in world units)
    float gameWidth = 10.0f;
    float gameHeight = 8.0f;
    float paddleWidth = 0.3f;
    float paddleHeight = 1.5f;
    float ballSize = 0.3f;

    // Positions and velocities (game coordinates)
    Vector3 leftPaddlePos;  // center of left paddle
    Vector3 rightPaddlePos; // center of right paddle
    Vector3 ballPos;
    Vector3 ballVel;  // velocity for ball

    // Paddle movement speed (units per second)
    float paddleSpeed = 5.0f;
    // Ball speed multiplier
    float ballSpeed = 3.0f;

protected:
    void OnInit() override {
        KOSMIC_INFO("[Pong] OnInit called.");
        renderer.Init();
        
        // Setup camera for orthographic view
        camera = std::make_shared<Camera>(60.0f, 800.0f / 600.0f);
        camera->SetPosition({0.0f, 0.0f, 10.0f});
        camera->SetRotation(0.0f, -90.0f);
        // Orthographic projection; using half of gameHeight (8.0/2 = 4.0f)
        camera->SetOrthographic(true, 4.0f);
        renderer.SetCamera(camera);

        // Create game objects
        leftPaddle = MeshLibrary::Cube();
        rightPaddle = MeshLibrary::Cube();
        ball = MeshLibrary::Sphere();

        // Set initial game state
        ResetGameState();

        // Configure light
        auto shader = renderer.GetShader();
        shader->Bind();
        shader->SetVec3("u_AmbientLightColor", Vector3(1.0f, 1.0f, 1.0f));
        shader->SetFloat("u_AmbientLightIntensity", 0.7f);
        shader->SetVec3("u_DirLightDirection", Vector3(-0.2f, -1.0f, -0.3f));
        shader->SetVec3("u_DirLightColor", Vector3(1.0f, 1.0f, 1.0f));
        shader->SetFloat("u_DirLightIntensity", 0.3f);
        shader->Unbind();
        
        KOSMIC_INFO("[Pong] Initialization complete");
    }

private:
    void ResetGameState() {
        // Reset positions
        leftPaddlePos = Vector3(-gameWidth * 0.45f, 0.0f, 0.0f);
        rightPaddlePos = Vector3(gameWidth * 0.45f, 0.0f, 0.0f);
        ballPos = Vector3(0.0f, 0.0f, 0.0f);
        
        // Initial ball velocity
        float angle = (std::rand() % 120 - 60) * (PI / 180.0f); // Random angle between -60 and 60 degrees
        ballVel = Vector3(std::cos(angle), std::sin(angle), 0.0f) * ballSpeed;
        if (std::rand() % 2) ballVel.x = -ballVel.x; // Random initial direction
    }

    // Update game logic
    void OnUpdate(float deltaTime) override {
        // Left Paddle control (W and S keys)
        if (Input::IsKeyPressed(SDLK_w))
            leftPaddlePos.y += paddleSpeed * deltaTime;
        if (Input::IsKeyPressed(SDLK_s))
            leftPaddlePos.y -= paddleSpeed * deltaTime;

        // Right Paddle control (Up and Down arrow keys)
        if (Input::IsKeyPressed(SDLK_UP))
            rightPaddlePos.y += paddleSpeed * deltaTime;
        if (Input::IsKeyPressed(SDLK_DOWN))
            rightPaddlePos.y -= paddleSpeed * deltaTime;

        // Clamp paddles to game bounds
        float paddleLimit = gameHeight * 0.5f - paddleHeight * 0.5f;
        if (leftPaddlePos.y > paddleLimit) leftPaddlePos.y = paddleLimit;
        if (leftPaddlePos.y < -paddleLimit) leftPaddlePos.y = -paddleLimit;
        if (rightPaddlePos.y > paddleLimit) rightPaddlePos.y = paddleLimit;
        if (rightPaddlePos.y < -paddleLimit) rightPaddlePos.y = -paddleLimit;

        // Update ball position
        ballPos = ballPos + ballVel * deltaTime;

        // Collision with top and bottom walls
        if (ballPos.y + ballSize * 0.5f > gameHeight * 0.5f || ballPos.y - ballSize * 0.5f < -gameHeight * 0.5f) {
            ballVel.y = -ballVel.y;
        }

        // Check collision with left paddle
        if (ballPos.x - ballSize * 0.5f < leftPaddlePos.x + paddleWidth * 0.5f) {
            if (ballPos.y < leftPaddlePos.y + paddleHeight * 0.5f &&
                ballPos.y > leftPaddlePos.y - paddleHeight * 0.5f) {
                ballVel.x = fabs(ballVel.x);
            }
        }
        // Check collision with right paddle
        if (ballPos.x + ballSize * 0.5f > rightPaddlePos.x - paddleWidth * 0.5f) {
            if (ballPos.y < rightPaddlePos.y + paddleHeight * 0.5f &&
                ballPos.y > rightPaddlePos.y - paddleHeight * 0.5f) {
                ballVel.x = -fabs(ballVel.x);
            }
        }

        // If ball goes out of left or right bounds, reset it
        if (ballPos.x < -gameWidth * 0.5f || ballPos.x > gameWidth * 0.5f) {
            ballPos = Vector3(0.0f, 0.0f, 0.0f);
            // Restart ball with opposite direction
            ballVel = Normalize(Vector3(-ballVel.x, ballVel.y, 0.0f)) * ballSpeed;
        }
    }

    // Render game objects
    void OnRender() override {
        // Clear the screen (the renderer's Render will clear buffers)
        renderer.Render();

        // Get the shader from renderer and bind it for manual drawing
        auto shader = renderer.GetShader();
        shader->Bind();
        // Set view and projection matrices from camera
        shader->SetMat4("view", camera->GetViewMatrix());
        shader->SetMat4("projection", camera->GetProjectionMatrix());
        shader->SetInt("u_Texture", 0); // default texture unit

        // Left Paddle transform: scale then translate
        Mat4 leftPaddleTransform = Scale(Translate(Mat4(1.0f), { leftPaddlePos.x, leftPaddlePos.y, 0.0f }), { paddleWidth, paddleHeight, 1.0f });
        leftPaddle->SetTransform(leftPaddleTransform);
        shader->SetMat4("model", leftPaddleTransform);
        shader->SetVec4("u_Color", Math::Vector4(1.0f, 0.2f, 0.2f, 1.0f)); // Red for left paddle
        leftPaddle->Draw();

        // Right Paddle transform
        Mat4 rightPaddleTransform = Scale(Translate(Mat4(1.0f), { rightPaddlePos.x, rightPaddlePos.y, 0.0f }), { paddleWidth, paddleHeight, 1.0f });
        rightPaddle->SetTransform(rightPaddleTransform);
        shader->SetMat4("model", rightPaddleTransform);
        shader->SetVec4("u_Color", Math::Vector4(0.2f, 0.2f, 1.0f, 1.0f)); // Blue for right paddle
        rightPaddle->Draw();

        // Ball transform
        Mat4 ballTransform = Scale(Translate(Mat4(1.0f), { ballPos.x, ballPos.y, 0.0f }), { ballSize, ballSize, 1.0f });
        ball->SetTransform(ballTransform);
        shader->SetMat4("model", ballTransform);
        shader->SetVec4("u_Color", Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f)); // White for ball
        ball->Draw();

        shader->Unbind();
    }

    // Cleanup
    void OnCleanup() override {}
};

int main() {
    Log::Init();
    KOSMIC_INFO("[Pong] Starting PongApp...");
    PongApp app;
    app.Run();
    KOSMIC_INFO("[Pong] PongApp terminated.");
    return 0;
}
