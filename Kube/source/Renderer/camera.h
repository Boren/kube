#pragma once

#include <glm/glm.hpp>

//! \brief Contains information about projections and functions to move it
//! \todo Move some code to separate frustum class
class Camera {
public:
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();
    glm::vec3 getPosition() { return m_cameraPosition; }
    float getHorizontalAngle() { return m_horizontalAngle; }
    float getVerticalAngle() { return m_verticalAngle; }

	void setPerspectiveProjection(float fieldOfView, float aspectRatio);
	void setOrthogonalProjection(float windowWidth, float windowHeight);

	void setPosition(glm::vec3 newPosition);
	void setHorizontalAngle(float radians);
	void setVerticalAngle(float radians);

	void lookAt(glm::vec3 point);

	void moveCurrentDirection(float distance);
	void strafe(float distance);
	void rotate(int x, int y);

private:
	void update();
    glm::vec3 m_cameraPosition = glm::vec3(16, 16, -32);
    float m_horizontalAngle = 0.0f;
    float m_verticalAngle = 0.0f;

	glm::vec3 m_vectorUp;
	glm::vec3 m_vectorRight;
	glm::vec3 m_vectorForward;

	glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
};
