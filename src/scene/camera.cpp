/// @class Camera
/// @brief Contains information about projections and functions to move camera
/// @todo Move some code to separate frustum class

#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

/// @brief Constructs a new camera.
/// Z-vector will be positive up.
/// Y-vector will be positive in
/// X-vector will be positive right
Camera::Camera() {
    m_vectorUp = glm::vec3(0.0f, 0.0f, 1.0f);
    m_vectorForward = glm::vec3(0.0f, 1.0f, 0.0f);
    m_vectorRight = glm::vec3(1.0f, 0.0f, 0.0f);
}

/// @brief Updates vectors and view matrix to latest values. Should be run after updating yaw or pitch.
void Camera::update() {
    glm::vec3 front;
    front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    front.z = glm::sin(glm::radians(m_pitch));
    front.y = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
    m_vectorForward = glm::normalize(front);

    m_vectorRight = glm::normalize(glm::cross(m_vectorForward, m_vectorUp));

    m_viewMatrix = glm::lookAt(m_cameraPosition,
                               m_cameraPosition + m_vectorForward, m_vectorUp);
}

/// @brief Moves the camera along the forward vector (current direction)
/// @param distance Units to move.
void Camera::moveCurrentDirection(float distance) {
    m_cameraPosition += m_vectorForward * distance;
}

/// @brief Moves the camera along the right vector
/// @param distance Units to move.
void Camera::strafe(float distance) {
    m_cameraPosition += m_vectorRight * distance;
}

/// @brief Moves camera to a set position
/// All vectors and rotation values are preserved.
/// @param newPosition Position to move to
void Camera::setPosition(glm::vec3 newPosition) {
    m_cameraPosition = newPosition;
}

/// @brief Returns the current yaw angle of the camera
/// @return Current yaw value
GLfloat Camera::getYaw() {
    return m_yaw;
}

/// @brief Returns the current pitch angle of the camera
/// @return Current pitch value
GLfloat Camera::getPitch() {
    return m_pitch;
}

/// @brief Sets the yaw of the camera to a fixed angle and updates vectors
/// @param yaw Yaw angle to set
void Camera::setYaw(GLfloat yaw) {
    m_yaw = yaw;

    update();
}

/// @brief Sets the pitch of the camera to a fixed angle and updates vectors
/// Note: This value will be clamped between 90 and -90
/// @param pitch Pitch angle to set
void Camera::setPitch(GLfloat pitch) {
    m_pitch = pitch;

    if(m_pitch > 90.0f)
        m_pitch = 90.0f;
    if(m_pitch < -90.0f)
        m_pitch = -90.0f;

    update();
}

/// @brief Updates projection matrix to a perspective one with given values
/// @param fieldOfView Field of view
/// @param aspectRatio Aspect ratio
void Camera::setPerspectiveProjection(float fieldOfView, float aspectRatio) {
    m_projectionMatrix =
            glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 1500.0f);
}

/// @brief Returns the current projection matrix
/// @return Current projection matrix
glm::mat4 Camera::getProjectionMatrix() { return m_projectionMatrix; }

/// @brief Updates vectors and returns the current view matrix
/// @return Current view matrix
glm::mat4 Camera::getViewMatrix() {
    update();
    return m_viewMatrix;
}

/// @brief Returns the current position of the camera
/// @return Current position of camera
glm::vec3 Camera::getPosition() {
    return m_cameraPosition;
}
