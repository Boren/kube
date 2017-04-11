#include "kubeEngine.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "chunks/block.h"
#include "utils/log.h"
#include "renderer/text2d.h"

Engine::Engine()
{
								// Output version numbers
								int glfwMajor, glfwMinor, glfwRev;
								glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRev);

								std::stringstream ss;
								ss << "GLFW Version: " << glfwMajor << "." << glfwMinor << "." << glfwRev;
								Log::message(Log::DEBUG, "Engine", ss.str());

								// Output size of important classes for optimization debug
								ss.str("");
								ss << "Size of block: " << sizeof(Block);
								Log::message(Log::DEBUG, "Engine", ss.str());

								ss.str("");
								ss << "Size of chunk: " << sizeof(Chunk);
								Log::message(Log::DEBUG, "Engine", ss.str());
}

bool Engine::initialize(std::string windowName)
{
								// Initialize window
								pWindow = new Window();
								if (!pWindow->create(false, windowName)) {
																Log::message(Log::FATAL, "Engine", "Window init failed");
																return false;
								}

								// Load OpenGL
								if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
																Log::message(Log::FATAL, "Engine", "OpenGL init failed");
																return false;
								}

								std::stringstream ss;
								ss << "Renderer: " << glGetString(GL_VENDOR) << " - " << glGetString(GL_RENDERER);
								Log::message(Log::DEBUG, "Engine", ss.str());

								ss.str("");
								ss << "OpenGL " << glGetString(GL_VERSION) << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION);
								Log::message(Log::DEBUG, "Engine", ss.str());


								// Initialize renderer
								pRenderer = new Renderer();
								if(!pRenderer->initialize(pWindow->getWidth(), pWindow->getHeight())) {
																Log::message(Log::FATAL, "Engine", "Render init failed");
																return false;
								}

								// Initialize scene manager
								pSceneManager = new SceneManager();

								initText2D("fonts/CubeFontFilledInvert.dds");
								Text2DupdateResolution(pWindow->getWidth(), pWindow->getHeight());

								return true;
}

void Engine::setSceneManager(SceneManager * sceneManager)
{
								pSceneManager = sceneManager;
}

SceneManager* Engine::getSceneManager() {
								return pSceneManager;
}

Renderer* Engine::getRenderer() {
								return pRenderer;
}


void Engine::setCamera(Camera *camera) {
								pCamera = camera;
}

Window* Engine::getWindow()
{
								return pWindow;
}

bool Engine::update(float deltaTime)
{
								if (glfwGetKey(pWindow->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS
												|| glfwWindowShouldClose(pWindow->getWindow())) {
																return false;
																glfwTerminate();
								}

								pWindow->update();
								pRenderer->rotateAngle = 0.0f;
								pSceneManager->update(deltaTime, pCamera);
								pRenderer->render(pSceneManager, pCamera);
								pWindow->render();

								return true;
}

float Engine::getDeltaTime() {
								float currentTime = (float)glfwGetTime();
								float deltaTime = currentTime - m_previousTime;

								m_previousTime = currentTime;

								return deltaTime;
}
