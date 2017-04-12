#include "sample.h"
#include <iostream>
#include <utils/log.h>
#define _USE_MATH_DEFINES
#include <math.h>

int main(int, char **) {
  Sample app;

  app.run();

  return 0;
}

void Sample::run() {
  m_engine = new Engine();

  // Initialize Kube engine
  if (!m_engine->initialize("OpenGL Block Engine - Point Light Testing")) {
    Log::message(Log::FATAL, "Main", "Failed to initialize the engine.");
    return;
  }

  // Create a camera and move it to where we want it
  m_camera = new Camera();
  m_camera->setPerspectiveProjection(45.0f,
                                     (float)m_engine->getWindow()->getWidth() /
                                         m_engine->getWindow()->getHeight());
  m_camera->setPosition(glm::vec3(-20.0f, 55.0f, -38.0f));
  m_camera->setVerticalAngle(-(float)M_PI / 5.5f);
  m_camera->setHorizontalAngle((float)M_PI / 5.0f);

  m_engine->setCamera(m_camera);

  m_chunkManager = new ChunkManager(0, 0, false);
  m_engine->getSceneManager()->setChunkManager(m_chunkManager);

  // Set up a basic light source (sun)
  Light *sun = new Light();
  sun->position = glm::vec4(1, 0.8, -0.6, 0);
  sun->color = glm::vec3(1, 1, 1);
  sun->ambientStrength = 0.1f;
  // m_engine->getSceneManager()->addLight(sun);

  // Set up a test point light
  Light *pointLight = new Light();
  pointLight->position = glm::vec4(-8, 40, -20, 1);
  pointLight->color = glm::vec3(1, 1, 1);
  pointLight->ambientStrength = 0.0f;
  pointLight->attenuation = 0.0f;
  m_engine->getSceneManager()->addLight(pointLight);

  m_engine->getSceneManager()->getChunkManager()->setChunkLoadCallback(
      std::bind(&Sample::onChunkLoad, this, std::placeholders::_1));

  while (true) {
    float deltaTime = m_engine->getDeltaTime();
    float r = glm::mix(0.0f, 2.0f, fmod(glfwGetTime() / 7.13f, 1.0f));
    if (r > 1.0f)
      r = 1.0f - (r - 1.0f);
    float g = glm::mix(2.0f, 0.0f, fmod(glfwGetTime() / 5.36f, 1.0f));
    if (g > 1.0f)
      g = 1.0f - (g - 1.0f);
    float b = glm::mix(0.0f, 2.0f, fmod(glfwGetTime() / 2.23f, 1.0f));
    if (b > 1.0f)
      b = 1.0f - (b - 1.0f);

    pointLight->color = glm::vec3(r, g, b);

    float y = glm::mix(15.0f, 35.0f, fmod(glfwGetTime() / 4.0f, 1.0f));
    if (y > 25.0f)
      y = 25.0f - (y - 25.0f);

    float z = glm::mix(-10.0f, 20.0f, fmod(glfwGetTime() / 6.0f, 1.0f));
    if (z > 5.0f)
      z = 5.0f - (z - 5.0f);

    pointLight->position = glm::vec4(-6, y, z, 1);
    if (!m_engine->update(deltaTime))
      break;
  }
}

void Sample::onChunkLoad(Chunk *chunk) {
  for (int x = 0; x < ChunkManager::CHUNK_SIZE; x++) {
    for (int z = 0; z < ChunkManager::CHUNK_SIZE; z++) {
      for (int y = 0; y < ChunkManager::CHUNK_SIZE; y++) {
        if ((sqrt((z - ChunkManager::CHUNK_SIZE / 2) *
                      (z - ChunkManager::CHUNK_SIZE / 2) +
                  (x - ChunkManager::CHUNK_SIZE / 2) *
                      (x - ChunkManager::CHUNK_SIZE / 2) +
                  (y - ChunkManager::CHUNK_SIZE / 2) *
                      (y - ChunkManager::CHUNK_SIZE / 2))) <
            ChunkManager::CHUNK_SIZE / 2) {
          chunk->getBlock(glm::ivec3(x, y, z))->setColorNum(1);
        } else {
          chunk->getBlock(glm::ivec3(x, y, z))->setColorNum(0);
        }
      }
    }
  }
}
