#include "Kosmic/Renderer/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Kosmic::Renderer {

Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
    : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearPlane(nearPlane), m_FarPlane(farPlane) {
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
    
    glm::vec3 glmFront = glm::normalize(glm::vec3(front.x, front.y, front.z));
    m_Front = { glmFront.x, glmFront.y, glmFront.z };

    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix() {
    glm::vec3 position(m_Position.x, m_Position.y, m_Position.z);
    glm::vec3 target((m_Position + m_Front).x, (m_Position + m_Front).y, (m_Position + m_Front).z);
    glm::vec3 up(m_Up.x, m_Up.y, m_Up.z);
    m_ViewMatrix = glm::lookAt(position, target, up);
}

void Camera::UpdateProjectionMatrix() {
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
}

void Camera::SetFOV(float fov) {
    m_FOV = fov;
    UpdateProjectionMatrix();
}

void Camera::SetAspectRatio(float ratio) {
    m_AspectRatio = ratio;
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
    glm::mat4 view = m_ViewMatrix;
    view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return view;
}

} // namespace Kosmic::Renderer
