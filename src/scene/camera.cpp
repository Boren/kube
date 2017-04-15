#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::strafe(float distance) {
  m_cameraPosition += m_vectorRight * distance;
}

void Camera::setPosition(glm::vec3 newPosition) {
  m_cameraPosition = newPosition;
}

void Camera::setHorizontalAngle(float radians) { m_horizontalAngle = radians; }

void Camera::setVerticalAngle(float radians) { m_verticalAngle = radians; }

//! \todo Look at currently resets every camera update
void Camera::lookAt(glm::vec3 point) {
  m_viewMatrix = glm::lookAt(m_cameraPosition, point, m_vectorUp);
}

void Camera::moveCurrentDirection(float distance) {
  m_cameraPosition += m_vectorForward * distance;
}

void Camera::rotate(int x, int y) {
  m_horizontalAngle += 0.002f * x;
  m_verticalAngle += 0.002f * y;
}

void Camera::update() {
  // Direction: Spherical coordinates to Cartesian coordinates conversion
  m_vectorForward = glm::vec3(cos(m_verticalAngle) * sin(m_horizontalAngle),
                              sin(m_verticalAngle),
                              cos(m_verticalAngle) * cos(m_horizontalAngle));

  // Right vector
  m_vectorRight = glm::vec3(sin(m_horizontalAngle - 3.14f / 2.0f), 0,
                            cos(m_horizontalAngle - 3.14f / 2.0f));

  // Up vector: Perpendicular to both direction and right
  m_vectorUp = glm::cross(m_vectorRight, m_vectorForward);

  m_viewMatrix = glm::lookAt(m_cameraPosition,
                             m_cameraPosition + m_vectorForward, m_vectorUp);
}

void Camera::setPerspectiveProjection(float fieldOfView, float aspectRatio) {
  m_projectionMatrix =
      glm::perspective(glm::radians(fieldOfView), aspectRatio, 0.1f, 1500.0f);
}

void Camera::setOrthogonalProjection(float windowWidth, float windowHeight) {
  m_projectionMatrix =
      glm::ortho(0.0f, windowWidth, windowHeight, 0.0f, 0.1f, 1500.0f);
}

glm::mat4 Camera::getProjectionMatrix() { return m_projectionMatrix; }

glm::mat4 Camera::getViewMatrix() {
  update();
  return m_viewMatrix;
}
