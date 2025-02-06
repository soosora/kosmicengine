#pragma once
#include "Kosmic/Core/Math/Math.hpp"

namespace Kosmic::Renderer {

class Camera {
public:
    Camera(float fov = 45.0f, float aspectRatio = 16.0f/9.0f, float nearPlane = 0.1f, float farPlane = 100.0f);

    // Camera movement
    void SetPosition(const Math::Vector3& position);
    void SetRotation(float pitch, float yaw);
    
    // Camera properties
    void SetFOV(float fov);
    void SetAspectRatio(float ratio);

    // Get matrices and vectors
    const Math::Mat4& GetViewMatrix() const { return m_ViewMatrix; }
    const Math::Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const Math::Vector3& GetPosition() const { return m_Position; }
    const Math::Vector3& GetFront() const { return m_Front; }

    float GetPitch() const;
    float GetYaw() const;

private:
    Math::Vector3 m_Position{0.0f, 0.0f, 3.0f};
    Math::Vector3 m_Front{0.0f, 0.0f, -1.0f};
    Math::Vector3 m_Up{0.0f, 1.0f, 0.0f};
    
    float m_Pitch{0.0f};
    float m_Yaw{-90.0f};
    
    float m_FOV;
    float m_AspectRatio;
    float m_NearPlane;
    float m_FarPlane;

    Math::Mat4 m_ViewMatrix{1.0f};
    Math::Mat4 m_ProjectionMatrix{1.0f};
    
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
};

} // namespace Kosmic::Renderer
