#pragma once

#include "noise.h"
#include "noise/noiseutils.h"
#include <kubeEngine.h>

class Sample {
public:
  void run();
  void onChunkLoad(Chunk *chunk);
  void toggleRenderMode();

private:
  ChunkManager *m_chunkManager;
  Engine *m_engine;
  Camera *m_camera;

  module::Perlin noiseGenerator;
  utils::NoiseMap heightMap;
  utils::NoiseMapBuilderPlane heightMapBuilder;
};
