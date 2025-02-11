#include "Kosmic/Renderer/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Kosmic::Renderer {

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane, bool isOrtho, float orthoSize)
    : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane),
      m_IsOrthographic(isOrtho), m_OrthoSize(orthoSize) {
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Camera::SetPosition(const Math::Vector3& position) {
    m_Position = position;
    UpdateViewMatrix();
}

void Camera::SetRotation(float pitch, float yaw) {
    m_Pitch = pitch;
    m_Yaw = yaw;

    // Calculate new Front vector
    Math::Vector3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    Math::Vector3 glmFront = Math::Normalize(Math::Vector3(front.x, front.y, front.z));
    m_Front = { glmFront.x, glmFront.y, glmFront.z };

    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix() {
    Math::Vector3 position(m_Position.x, m_Position.y, m_Position.z);
    Math::Vector3 target((m_Position + m_Front).x, (m_Position + m_Front).y, (m_Position + m_Front).z);
    Math::Vector3 up(m_Up.x, m_Up.y, m_Up.z);
    m_ViewMatrix = Math::LookAt(position, target, up);
}

void Camera::UpdateProjectionMatrix() {
    if (m_IsOrthographic) {
        // Use orthoSize as half-height, calculate half-width based on aspect ratio
        float orthoHalfHeight = m_OrthoSize;
        float orthoHalfWidth = orthoHalfHeight * m_AspectRatio;
        m_ProjectionMatrix = glm::ortho(-orthoHalfWidth, orthoHalfWidth, -orthoHalfHeight, orthoHalfHeight, m_NearPlane, m_FarPlane);
    } else {
        m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
    }
}

void Camera::SetFOV(float fov) {
    m_FOV = fov;
    UpdateProjectionMatrix();
}

void Camera::SetAspectRatio(float ratio) {
    m_AspectRatio = ratio;
    UpdateProjectionMatrix();
}

void Camera::SetOrthographic(bool isOrtho, float orthoSize) {
    // Enable or disable orthographic projection and set ortho size
    m_IsOrthographic = isOrtho;
    m_OrthoSize = orthoSize;
    UpdateProjectionMatrix();
}

float Camera::GetPitch() const {
    return m_Pitch;
}

float Camera::GetYaw() const {
    return m_Yaw;
}

// Get view matrix without translation (skybox rendering)
const Math::Mat4 Camera::GetViewMatrixNoTranslation() const {
    Math::Mat4 view = m_ViewMatrix;
    view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return view;
}

const Math::Mat4 Camera::GetSkyboxProjectionMatrix() const {
    // Always use perspective projection for the skybox
    return glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
}

} // namespace Kosmic::Renderer
