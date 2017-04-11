#include <glad/glad.h>
#include <iostream>
#include <iomanip>
#include "chunkManager.h"

#include <sstream>
#include "utils/log.h"

ChunkManager::ChunkManager(int loadDistance, int viewDistance, bool dynamicLoading) {
	setLoadDistance(loadDistance);
	setViewDistance(viewDistance);
	setDynamicLoading(dynamicLoading);
}

void ChunkManager::update(float deltaTime, Camera* camera) {
	glm::ivec3 currentChunkCoord(0, 0, 0);

	if (m_dynamicLoading) {
		glm::vec3 cameraPosition = camera->getPosition();

		currentChunkCoord = glm::ivec3(floor(cameraPosition.x / CHUNK_SIZE),
			floor(cameraPosition.y / CHUNK_SIZE),
			floor(cameraPosition.z / CHUNK_SIZE));
	}

	for (int x = currentChunkCoord.x - m_loadDistance; x <= currentChunkCoord.x + m_loadDistance; x++) {
		for (int y = currentChunkCoord.y - m_loadDistance; y <= currentChunkCoord.y + m_loadDistance; y++) {
			for (int z = currentChunkCoord.z - m_loadDistance; z <= currentChunkCoord.z + m_loadDistance; z++) {
				glm::ivec3 chunkCoord(glm::ivec3(x, y, z));

				if (m_chunks.count(chunkCoord) == 0) {
					m_chunks[chunkCoord] = new Chunk(chunkCoord);
					setNeighbors(m_chunks[chunkCoord]);

					m_chunks[chunkCoord]->load();
					if (callback_loadChunk != nullptr) {
						callback_loadChunk(m_chunks[chunkCoord]);
					}
				}
			}
		}
	}

	for (int x = currentChunkCoord.x - m_viewDistance; x <= currentChunkCoord.x + m_viewDistance; x++) {
		for (int y = currentChunkCoord.y - m_viewDistance; y <= currentChunkCoord.y + m_viewDistance; y++) {
			for (int z = currentChunkCoord.z - m_viewDistance; z <= currentChunkCoord.z + m_viewDistance; z++) {
				glm::ivec3 chunkCoord(glm::ivec3(x, y, z));

				if (m_chunks.find(chunkCoord) != m_chunks.end()) {
					if(m_chunks[chunkCoord]->isLoaded() && !m_chunks[chunkCoord]->isMeshed())
					m_chunks[chunkCoord]->mesh();
				}
			}
		}
	}
}

void ChunkManager::render() {
	for (auto iterator = m_chunks.begin(); iterator != m_chunks.end(); iterator++) {
		iterator->second->render();
	}
}

Chunk* ChunkManager::getChunk(glm::ivec3 chunkCoord) {
	return m_chunks.at(chunkCoord);
}

Block* ChunkManager::getBlock(glm::ivec3 chunkCoord, glm::ivec3 blockCoord) {
	return m_chunks.at(chunkCoord)->getBlock(blockCoord);
}

int ChunkManager::getNumberOfVertices()
{
	int numberOfVertices = 0;

	for(auto const &chunk : m_chunks) {
		numberOfVertices += chunk.second->m_numberOfVertices;
	}

	return numberOfVertices;
}

//! \todo Use spherical load distance
void ChunkManager::setLoadDistance(int loadDistance) {
	m_loadDistance = loadDistance; // *CHUNK_SIZE * BLOCK_SIZE;
	m_unloadDistance = (loadDistance + 1); // *CHUNK_SIZE * BLOCK_SIZE;
}

//! \todo Use spherical view distance
void ChunkManager::setViewDistance(int viewDistance) {
	m_viewDistance = viewDistance; // *CHUNK_SIZE * BLOCK_SIZE;
}

void ChunkManager::setDynamicLoading(bool dynamicLoading)
{
	m_dynamicLoading = dynamicLoading;
}

void ChunkManager::setChunkLoadCallback(std::function<void(Chunk*)> callbackFunction)
{
	callback_loadChunk = callbackFunction;
}

void ChunkManager::setNeighbors(Chunk *chunk)
{
	glm::ivec3 coord = chunk->m_chunkCoord;

	if (m_chunks.count(coord + glm::ivec3(0, 0, 1))) {
		chunk->m_left = m_chunks.at(coord + glm::ivec3(0, 0, 1));
		m_chunks.at(coord + glm::ivec3(0, 0, 1))->m_right = chunk;
	}

	if (m_chunks.count(coord + glm::ivec3(0, 0, -1))) {
		chunk->m_right = m_chunks.at(coord + glm::ivec3(0, 0, -1));
		m_chunks.at(coord + glm::ivec3(0, 0, -1))->m_left = chunk;
	}

	if (m_chunks.count(coord + glm::ivec3(0, 1, 0))) {
		chunk->m_top = m_chunks.at(coord + glm::ivec3(0, 1, 0));
		m_chunks.at(coord + glm::ivec3(0, 1, 0))->m_bottom = chunk;
	}

	if (m_chunks.count(coord + glm::ivec3(0, -1, 0))) {
		chunk->m_bottom = m_chunks.at(coord + glm::ivec3(0, -1, 0));
		m_chunks.at(coord + glm::ivec3(0, -1, 0))->m_top = chunk;
	}

	if (m_chunks.count(coord + glm::ivec3(1, 0, 0))) {
		chunk->m_front = m_chunks.at(coord + glm::ivec3(1, 0, 0));
		m_chunks.at(coord + glm::ivec3(1, 0, 0))->m_back = chunk;
	}

	if (m_chunks.count(coord + glm::ivec3(-1, 0, 0))) {
		chunk->m_back = m_chunks.at(coord + glm::ivec3(-1, 0, 0));
		m_chunks.at(coord + glm::ivec3(-1, 0, 0))->m_front = chunk;
	}
}
