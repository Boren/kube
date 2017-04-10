#pragma once

#include <vector>
#include "light.h"
#include "Chunks/chunkManager.h"
#include "Renderer/renderer.h"
#include "shader.h"

class Renderer;

//! \brief Controls the current scene and holds all objects which is part of the rendering
class SceneManager {
public:
    //! Default Constructor
    SceneManager();

    void update(float deltaTime, Camera* camera);

    void render(Renderer *renderer);

    void addLight(Light *light);

    void setChunkManager(ChunkManager *chunkManager);
    ChunkManager* getChunkManager();

private:
    ChunkManager *pChunkManager;
    std::vector<Light*> m_lights; //!< Contains a reference to all lights in the scene
    bool isLightsChanged = true;
    void updateLightUniform(Shader *shader);
};
