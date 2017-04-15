#pragma once

#include "../chunks/chunkManager.h"
#include "light.h"
#include "../renderer/renderer.h"
#include "../renderer/shader.h"
#include "voxelModel.h"
#include <vector>

class Renderer;

//! \brief Controls the current scene and holds all objects which is part of the
//! rendering
class SceneManager {
public:
    //! Default Constructor
    SceneManager();

    void update(float deltaTime, Camera *camera);

    void render(Renderer *renderer);

    void addLight(Light *light);

    void setChunkManager(ChunkManager *chunkManager);

    ChunkManager *getChunkManager();

    void addVoxelModel(VoxelModel *model);

private:
    ChunkManager *pChunkManager;
    std::vector<VoxelModel *> m_voxelModels;
    std::vector<Light *>
            m_lights; //!< Contains a reference to all lights in the scene
    bool isLightsChanged = true;

    void updateLightUniform(Shader *shader);
};
