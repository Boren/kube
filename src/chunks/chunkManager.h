#pragma once

#include <functional>
#include "chunk.h"
#include "block.h"
#include "renderer/camera.h"
#include <unordered_map>
#include <glm/glm.hpp>

//! Creates a hash of glm::ivec3 for hashmap functionality
struct ivec3KeyHasher
{
	std::size_t operator()(const glm::ivec3& key) const
	{
		using std::size_t;
		using std::hash;

		return ((key.x * 1337) ^ (key.y * 2674)) ^ (key.z * 5348);
	}
};

//! Keeps track of all chunks and is responsible for loading and unloading
//! \todo Thread based loading
//! \todo Chunk unloading
class ChunkManager {
public:
    //! Default Constructor
    /*!
      \param loadDistance Integer designating how many chunks should be loaded from camera position.
      \param viewDistance Integer designating how many chunks should be visible from camera position.
      \remark Setting view distance lower than load distance limits view distance to load distance.
    */
    ChunkManager(int loadDistance, int viewDistance, bool dynamicLoading);

    //!	Updates active chunks each frame.
    /*!
      \param deltaTime Time passed since last frame.
      \param camera Reference to camera object containing information about current viewport.
      \remark This function is automatically called each frame by the engine.
	*/
	void update(float deltaTime, Camera* camera);

    //! Renders chunks in current view distance.
    /*!
      \remark This function is automatically called each frame by the engine.
	*/
	void render();

    //! Get chunk at specified location.
    /*!
      \param chunkCoord Location of chunk to get.
	*/
	Chunk* getChunk(glm::ivec3 chunkCoord);

    //! Get block at specified location.
    /*!
      \param chunkCoord Location of chunk to get.
      \param blockCoord Location of block to get.
	*/
	Block* getBlock(glm::ivec3 chunkCoord, glm::ivec3 blockCoord);

	int getNumberOfVertices();


	void setLoadDistance(int loadDistance);
	void setViewDistance(int viewDistance);
	void setDynamicLoading(bool dynamicLoading);

	void setChunkLoadCallback(std::function<void(Chunk*)> callbackFunction);

public:
    static const int CHUNK_SIZE = 32;

private:
    std::unordered_map<glm::ivec3, Chunk*, ivec3KeyHasher> m_chunks;

	std::function<void(Chunk*)> callback_loadChunk; //!< Function to be called when chunk is to be loaded

	void setNeighbors(Chunk *chunk);

    int m_loadDistance;
    int m_unloadDistance;
    int m_viewDistance;

    bool m_dynamicLoading;
};
