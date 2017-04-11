#pragma once

#include <KubeEngine.h>

class Sample {
public:
	void run();

	void onChunkLoad(Chunk * chunk);

private:
	ChunkManager* m_chunkManager;
	Engine* m_engine;
	Camera* m_camera;
};
