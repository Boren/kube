#include "kubeWindow.h"
#include "utils/log.h"
#include <math.h>
#include <iostream>

//void windowResizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
//void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

Window::Window() {
	// Default window parameters
	m_windowWidth = 1280;
	m_windowHeight = 720;
}

bool Window::create(bool createFullscreen, std::string windowName) {
	// Initialise GLFW
	if (!glfwInit()) {
		Log::message(Log::FATAL, "Renderer", "GLFW init failed");
		return false;
	}

	// Specify OpenGL version 3.2 and 8x multisampling
	glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glGetIntegerv(GL_SAMPLES_ARB, 8);

	// Open a window and create its OpenGL context
	if (createFullscreen) {
		// Monitor selection
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		GLFWmonitor* monitor = monitors[0];

		if (count > 1) {
			std::cout << "Select monitor (0-" << (count - 1) << "): ";
			int select;
			std::cin >> select;
			monitor = monitors[select];
		}

		pWindow = glfwCreateWindow(m_windowWidth, m_windowHeight,
            windowName.c_str(),
			monitor, NULL);
	}
	else {
		pWindow = glfwCreateWindow(m_windowWidth, m_windowHeight,
            windowName.c_str(),
			NULL, NULL);
	}

	if (pWindow == NULL) {
		Log::message(Log::FATAL, "Renderer", "Window creation failed");
		glfwTerminate();
		return false;
	}

	// Disable vertical sync
	glfwSwapInterval(0);
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, 1);
	glfwMakeContextCurrent(pWindow);

	// Resize callback
	//glfwSetWindowSizeCallback(pWindow, windowResizeCallback);

	return true;
}

void Window::update() {
	// Refresh cursor position
	double x, y;

	glfwGetCursorPos(pWindow, &x, &y);
	if (m_cursorLockedToCenter) {
		glfwSetCursorPos(pWindow, m_windowWidth / 2, m_windowHeight / 2);
	}

    m_cursorX = (int)floor(x);
    m_cursorY = (int)floor(y);
}

int Window::getWidth() {
	return m_windowWidth;
}

int Window::getHeight() {
	return m_windowHeight;
}

int Window::getCursorX() {
	return m_cursorX;
}

int Window::getCursorY() {
	return m_cursorY;
}

GLFWwindow* Window::getWindow() {
    return pWindow;
}

void Window::render() {
	glfwSwapBuffers(pWindow);
    glfwPollEvents();
}
