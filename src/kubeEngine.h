#pragma once

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include "chunks/chunkManager.h"
#include "renderer/renderer.h"
#include "kubeWindow.h"
#include "renderer/scenemanager.h"
#include "input/inputManager.h"

//! Main class keeping track of the whole engine
class Engine {
public:
Engine();

bool initialize(std::string windowName);
void initializeChunkManager(int numX, int numY, int numZ);
void setSceneManager(SceneManager *sceneManager);
SceneManager* getSceneManager();
InputManager* getInputManager();
Renderer* getRenderer();
void setCamera(Camera *camera);
Window* getWindow();

bool update(float deltaTime);

float getDeltaTime();

// Input
//void handleInput(float deltaTime);
//void keyPressed(int key, int scancode, int mods);
//void keyReleased(int key, int scancode, int mods);

private:
static Engine *instance;

Window *pWindow;
SceneManager *pSceneManager;
InputManager * pInputManager;
Renderer *pRenderer;
Camera *pCamera;

float m_previousTime = 0.0f;
};
