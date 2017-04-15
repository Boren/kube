#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera {
public:
    Camera();

    void moveCurrentDirection(float distance);
    void strafe(float distance);

    void setPerspectiveProjection(float fieldOfView, float aspectRatio);

    glm::vec3 getPosition();
    GLfloat getYaw();
    GLfloat getPitch();

    void setPosition(glm::vec3 newPosition);
    void setYaw(GLfloat yaw);
    void setPitch(GLfloat pitch);

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();

private:
    void update();

    glm::vec3 m_cameraPosition = glm::vec3(0.0f, -10.0f, 10.0f);

    GLfloat m_yaw = 90.0f;
    GLfloat m_pitch = 0.0f;

    glm::vec3 m_vectorUp;
    glm::vec3 m_vectorRight;
    glm::vec3 m_vectorForward;

    glm::mat4 m_projectionMatrix;
    glm::mat4 m_viewMatrix;
};
