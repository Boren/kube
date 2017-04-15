#include "renderer.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <numeric>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

bool Renderer::initialize(int windowWidth, int windowHeight) {
    m_windowHeight = windowHeight;
    m_windowWidth = windowWidth;

    // Set background color
    glClearColor(0 / 255.0f, 0 / 255.0f, 0 / 255.0f, 1);

    // Enable depth test
    // Accept fragment if it closer to the camera than the former one
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Cull back face to optimize rendering
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enable blending for transparency and text
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load default shader
    m_defaultShader.compileShader("assets/shaders/BasicPhong.vert", Shader::VERTEX);
    m_defaultShader.compileShader("assets/shaders/BasicPhong.frag",
                                  Shader::FRAGMENT);
    m_defaultShader.link();
    m_defaultShader.use();

    m_defaultShader.setUniform1i("ambientOcclusionEnabled", 1);

    return true;
}

void Renderer::render(SceneManager *sceneManager, Camera *camera,
                      TextManager *textManager) {

    double renderStartTime = glfwGetTime();

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 viewProjection =
            camera->getProjectionMatrix() * camera->getViewMatrix();

    m_defaultShader.use();
    m_defaultShader.setUniformMatrix4fv("camera", viewProjection);
    m_defaultShader.setUniformMatrix4fv("model", model);
    m_defaultShader.setUniform3f("cameraPosition", camera->getPosition().x,
                                 camera->getPosition().y,
                                 camera->getPosition().z);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (m_renderMode) {
        case RENDERMODE_SHADED:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;

        case RENDERMODE_WIREFRAME:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
    }

    sceneManager->render(this, &m_defaultShader);

    // Text rendering. Disable wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    char text[256];
    sprintf(text, "Render: %.2f ms (%.1f FPS)", m_profilingMean,
            1000 / m_profilingMean);
    textManager->renderText(text, 5.0f, 5.0f, 0.3f, glm::vec3(0.2f, 0.2f, 0.2f));

    sprintf(text, "Camera - X: %.1f Y: %.1f Z: %.1f", camera->getPosition().x,
            camera->getPosition().y, camera->getPosition().z);
    textManager->renderText(text, 5.0f, 23.0f, 0.3f, glm::vec3(0.2f, 0.2f, 0.2f));

    sprintf(text, "Camera - Yaw: %.3f Pitch: %.3f", camera->getYaw(), camera->getPitch());
    textManager->renderText(text, 5.0f, 41.0f, 0.3f, glm::vec3(0.2f, 0.2f, 0.2f));

    //sprintf(text, "Vertices: %d",
    //        sceneManager->getChunkManager()->getNumberOfVertices());
    //textManager->renderText(text, 5.0f, 41.0f, 0.3f, glm::vec3(0.2f, 0.2f, 0.2f));

    m_profiling.push_back((glfwGetTime() - renderStartTime) * 1000);

    if (glfwGetTime() > m_lastPrint + 0.3) {
        double sum = std::accumulate(m_profiling.begin(), m_profiling.end(), 0.0);
        m_profilingMean = sum / m_profiling.size();
        m_profiling.clear();
        m_lastPrint = glfwGetTime();
    }
}

void Renderer::setRenderMode(RenderMode mode) { m_renderMode = mode; }

RenderMode Renderer::getCurrentRenderMode() { return m_renderMode; }

Shader *Renderer::getDefaultShader() { return &m_defaultShader; }
