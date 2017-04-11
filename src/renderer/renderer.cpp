// Include standard headers
#include <iostream>
#include <fstream>
#include <vector>

#include <glad/glad.h>

// Include GLFW (Window Manager)
#include <GLFW/glfw3.h>

// Include GLM (OpenGL Mathematics)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iomanip>
#include <numeric>
#include <iostream>
#include <sstream>
#include <string>

#include "renderer.h"
#include "../chunks/chunkManager.h"
#include "text2d.h"
#include "shader.h"
#include "../utils/log.h"

bool Renderer::initialize(int windowWidth, int windowHeight) {
    m_windowHeight = windowHeight;
    m_windowWidth = windowWidth;

    // Set background color
    glClearColor(135/255.0f, 206/255.0f, 255/255.0f, 1);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull back face to optimize rendering
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Load default shader
	m_defaultShader.compileShader("shaders/VertexShader.vs", Shader::VERTEX);
	m_defaultShader.compileShader("shaders/FragmentShader.fs", Shader::FRAGMENT);
	m_defaultShader.link();
	m_defaultShader.use();

    m_defaultShader.setUniform1i("ambientOcclusionEnabled", 1);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    return true;
}

void Renderer::render(SceneManager *sceneManager, Camera *camera) {
    double renderStartTime = glfwGetTime();
    int numVertices = 0;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(8, 8, 8));
    model = glm::rotate(model, rotateAngle, glm::vec3(0,1,0));
    model = glm::translate(model, glm::vec3(-8, -8, -8));
    glm::mat4 viewProjection = camera->getProjectionMatrix() * camera->getViewMatrix();
    glm::mat4 MVP = camera->getProjectionMatrix() * camera->getViewMatrix() * model;

    m_defaultShader.use();
    m_defaultShader.setUniformMatrix4fv("camera", viewProjection);
    m_defaultShader.setUniformMatrix4fv("model", model);
    m_defaultShader.setUniformMatrix4fv("MVP", MVP);
	m_defaultShader.setUniform3f("cameraPosition", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (m_renderMode) {
	case RENDERMODE_SHADED:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case RENDERMODE_WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}

    sceneManager->render(this);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /*char text[256];
    sprintf(text,"Render: %.3f ms", m_profilingMean);
    printText2D(text, 10, m_windowHeight - 20, 16);

    text[256];
    sprintf(text,"FPS: %.0f", 1000 / m_profilingMean);
    printText2D(text, 10, m_windowHeight - 40, 16);

	text[256];
	sprintf(text, "Camera: X:%.0f Y:%.0f Z:%.0f", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
	printText2D(text, 10, m_windowHeight - 60, 16);

	text[256];
	sprintf(text, "Vertices: %d", sceneManager->getChunkManager()->getNumberOfVertices());
	printText2D(text, 10, m_windowHeight - 80, 16);

    m_profiling.push_back((glfwGetTime() - renderStartTime) * 1000);

    if(glfwGetTime() > m_lastPrint + 0.3) {
        double sum = std::accumulate(m_profiling.begin(), m_profiling.end(), 0.0);
        m_profilingMean = sum / m_profiling.size();
        m_profiling.clear();
        m_lastPrint = glfwGetTime();
    }*/
}

void Renderer::setRenderMode(RenderMode mode) {

}

Shader* Renderer::getDefaultShader() {
    return &m_defaultShader;
}
