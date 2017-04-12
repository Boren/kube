#include "scenemanager.h"
#include <iostream>
#include <sstream>
#include <string>

SceneManager::SceneManager() {
}

void SceneManager::setChunkManager(ChunkManager *chunkManager) {
  pChunkManager = chunkManager;
}

ChunkManager *SceneManager::getChunkManager() { return pChunkManager; }

void SceneManager::update(float deltaTime, Camera *camera) {
  pChunkManager->update(deltaTime, camera);
}

void SceneManager::addLight(Light *light) {
  m_lights.push_back(light);
  isLightsChanged = true;
}

void SceneManager::updateLightUniform(Shader *shader) {
  // Setup lights
  shader->setUniform1i("numLights", (int)m_lights.size());

  //!< \todo Incorporate this into shader class after generic shader uniform
  //! setting is implemented
  for (size_t i = 0; i < m_lights.size(); i++) {
    std::stringstream ss;
    ss << "lights[" << i << "].position";
    shader->setUniform4f(ss.str().c_str(), m_lights[i]->position.x,
                         m_lights[i]->position.y, m_lights[i]->position.z,
                         m_lights[i]->position.w);

    ss.str("");
    ss << "lights[" << i << "].color";
    shader->setUniform3f(ss.str().c_str(), m_lights[i]->color.r,
                         m_lights[i]->color.g, m_lights[i]->color.g);

    ss.str("");
    ss << "lights[" << i << "].attenuation";
    shader->setUniform1f(ss.str().c_str(), m_lights[i]->attenuation);

    ss.str("");
    ss << "lights[" << i << "].ambientStrength";
    shader->setUniform1f(ss.str().c_str(), m_lights[i]->ambientStrength);
  }
}

void SceneManager::render(Renderer *renderer) {
  if (isLightsChanged) {
    isLightsChanged = false;
  }

  updateLightUniform(renderer->getDefaultShader());

  pChunkManager->render();
}
