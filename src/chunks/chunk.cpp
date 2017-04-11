#include <iostream>
#include <vector>
#include <sstream>
#include <glm/glm.hpp>
#include "chunk.h"
#include "renderer/renderer.h"
#include "utils/log.h"

const float Chunk::BLOCK_SIZE = 1.0f;

Chunk::Chunk(glm::ivec3 chunkCoord) {
	m_chunkCoord = chunkCoord;
	m_back = nullptr;
	m_front = nullptr;
	m_bottom = nullptr;
	m_top = nullptr;
	m_left = nullptr;
	m_right = nullptr;
}

void Chunk::load() {
	std::stringstream ss;
	ss << "Loading chunk (" << m_chunkCoord.x << "," << m_chunkCoord.y << "," << m_chunkCoord.z << ")";
	Log::message(Log::DEBUG, "Chunk", ss.str());

	m_blocks = new Block[ChunkManager::CHUNK_SIZE * ChunkManager::CHUNK_SIZE * ChunkManager::CHUNK_SIZE]();

	m_isLoaded = true;
}

void Chunk::mesh()
{
	buildMesh();
}

void Chunk::update(float deltaTime) {

}

void Chunk::render() {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	// 4th attribute buffer : Ambient Occlusion
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, m_ambientOcclusionBuffer);
	glVertexAttribPointer(
		3,                                // attribute
		1,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, m_numberOfVertices);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

bool Chunk::isLoaded() {
	return m_isLoaded;
}

bool Chunk::isMeshed() {
	return m_isMeshed;
}

//! \todo Look at neighboring chunks
Block* Chunk::getBlock(glm::ivec3 coord) {
	if (coord.x < 0) {
		if (m_back == nullptr) return nullptr;
		return m_back->getBlock(coord + glm::ivec3(ChunkManager::CHUNK_SIZE, 0, 0));
	}
	if (coord.x >= ChunkManager::CHUNK_SIZE) {
		if (m_front == nullptr) return nullptr;
		return m_front->getBlock(coord - glm::ivec3(ChunkManager::CHUNK_SIZE, 0, 0));
	}

	if (coord.y < 0) {
		if (m_bottom == nullptr) return nullptr;
		return m_bottom->getBlock(coord + glm::ivec3(0, ChunkManager::CHUNK_SIZE, 0));
	}
	if (coord.y >= ChunkManager::CHUNK_SIZE) {
		if (m_top == nullptr) return nullptr;
		return m_top->getBlock(coord - glm::ivec3(0, ChunkManager::CHUNK_SIZE, 0));
	}

	if (coord.z < 0) {
		if (m_right == nullptr) return nullptr;
		return m_right->getBlock(coord + glm::ivec3(0, 0, ChunkManager::CHUNK_SIZE));
	}
	if (coord.z >= ChunkManager::CHUNK_SIZE) {
		if (m_left == nullptr) return nullptr;
		return m_left->getBlock(coord - glm::ivec3(0, 0, ChunkManager::CHUNK_SIZE));
	}

	return &m_blocks[coord.x * ChunkManager::CHUNK_SIZE * ChunkManager::CHUNK_SIZE +
		coord.y * ChunkManager::CHUNK_SIZE +
		coord.z];
}

void Chunk::buildMesh() {
	std::stringstream ss;
	ss << "Meshing chunk (" << m_chunkCoord.x << "," << m_chunkCoord.y << "," << m_chunkCoord.z << ")";
	Log::message(Log::DEBUG, "Chunk", ss.str());

	// Loop through each block
	for (int x = 0; x < ChunkManager::CHUNK_SIZE; x++) {
		for (int y = 0; y < ChunkManager::CHUNK_SIZE; y++) {
			for (int z = 0; z < ChunkManager::CHUNK_SIZE; z++) {

				Block* currentBlock = getBlock(glm::ivec3(x, y, z));

				// Only create mesh for active blocks
				if (currentBlock->isActive()) {
					createBlockMesh(currentBlock, glm::ivec3(ChunkManager::CHUNK_SIZE * m_chunkCoord.x + x,
						ChunkManager::CHUNK_SIZE * m_chunkCoord.y + y,
						ChunkManager::CHUNK_SIZE * m_chunkCoord.z + z),
						glm::ivec3(x, y, z));
				}
			}
		}
	}

	m_numberOfVertices = m_vertices.size();

	if (m_numberOfVertices != 0) {
		glGenBuffers(1, &m_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
		m_vertices.clear();

		glGenBuffers(1, &m_colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4), &m_colors[0], GL_STATIC_DRAW);
		m_colors.clear();

		glGenBuffers(1, &m_normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);
		m_normals.clear();

		glGenBuffers(1, &m_ambientOcclusionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_ambientOcclusionBuffer);
		glBufferData(GL_ARRAY_BUFFER, m_ambientOcclusionValues.size() * sizeof(float), &m_ambientOcclusionValues[0], GL_STATIC_DRAW);
		m_ambientOcclusionValues.clear();
	}

	m_isMeshed = true;
}

void Chunk::createBlockMesh(Block* block, glm::ivec3 blockCoord, glm::ivec3 blockCoordLocal) {
	float blockSizeHalf = BLOCK_SIZE / 2;

	glm::vec4 color = block->getColor();

	// Generate the points of the cube
	glm::vec3 p1(blockCoord.x - blockSizeHalf, blockCoord.y - blockSizeHalf, blockCoord.z + blockSizeHalf);
	glm::vec3 p2(blockCoord.x + blockSizeHalf, blockCoord.y - blockSizeHalf, blockCoord.z + blockSizeHalf);
	glm::vec3 p3(blockCoord.x + blockSizeHalf, blockCoord.y + blockSizeHalf, blockCoord.z + blockSizeHalf);
	glm::vec3 p4(blockCoord.x - blockSizeHalf, blockCoord.y + blockSizeHalf, blockCoord.z + blockSizeHalf);
	glm::vec3 p5(blockCoord.x + blockSizeHalf, blockCoord.y - blockSizeHalf, blockCoord.z - blockSizeHalf);
	glm::vec3 p6(blockCoord.x - blockSizeHalf, blockCoord.y - blockSizeHalf, blockCoord.z - blockSizeHalf);
	glm::vec3 p7(blockCoord.x - blockSizeHalf, blockCoord.y + blockSizeHalf, blockCoord.z - blockSizeHalf);
	glm::vec3 p8(blockCoord.x + blockSizeHalf, blockCoord.y + blockSizeHalf, blockCoord.z - blockSizeHalf);

	// We don't want to render sides which are occluded by other blocks so we only draw that side if the neighbor block is not active.
	if (!blockActive(blockCoordLocal + glm::ivec3(1, 0, 0))) createFaceMesh(glm::vec3(1, 0, 0), p2, p5, p8, p3, color, blockCoordLocal);
	if (!blockActive(blockCoordLocal + glm::ivec3(-1, 0, 0))) createFaceMesh(glm::vec3(-1, 0, 0), p6, p1, p4, p7, color, blockCoordLocal);
	if (!blockActive(blockCoordLocal + glm::ivec3(0, 1, 0))) createFaceMesh(glm::vec3(0, 1, 0), p4, p3, p8, p7, color, blockCoordLocal);
	if (!blockActive(blockCoordLocal + glm::ivec3(0, -1, 0))) createFaceMesh(glm::vec3(0, -1, 0), p6, p5, p2, p1, color, blockCoordLocal);
	if (!blockActive(blockCoordLocal + glm::ivec3(0, 0, 1))) createFaceMesh(glm::vec3(0, 0, 1), p1, p2, p3, p4, color, blockCoordLocal);
	if (!blockActive(blockCoordLocal + glm::ivec3(0, 0, -1))) createFaceMesh(glm::vec3(0, 0, -1), p5, p6, p7, p8, color, blockCoordLocal);
}

void Chunk::createFaceMesh(glm::vec3 normal, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec4 color, glm::ivec3 blockCoordLocal) {
	m_vertices.push_back(p1);
	m_vertices.push_back(p2);
	m_vertices.push_back(p3);

	m_vertices.push_back(p1);
	m_vertices.push_back(p3);
	m_vertices.push_back(p4);

	// Normals
	for (int i = 0; i < 6; i++) {
		m_normals.push_back(normal);
	}

	// Colors
	for (int i = 0; i < 6; i++) {
		m_colors.push_back(color);
	}

	// Ambient Occlusion
	for (int i = 0; i < 6; i++) {
		/*m_ambientOcclusionValues.push_back(getAmbientOcclusionAtVertex(blockCoordLocal, normal, glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
		m_ambientOcclusionValues.push_back(getAmbientOcclusionAtVertex(blockCoordLocal, normal, glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
		m_ambientOcclusionValues.push_back(getAmbientOcclusionAtVertex(blockCoordLocal, normal, glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
		m_ambientOcclusionValues.push_back(getAmbientOcclusionAtVertex(blockCoordLocal, normal, glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
		m_ambientOcclusionValues.push_back(getAmbientOcclusionAtVertex(blockCoordLocal, normal, glm::vec3(1, 0, 0), glm::vec3(0, 1, 0)));
		m_ambientOcclusionValues.push_back(getAmbientOcclusionAtVertex(blockCoordLocal, normal, glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));*/
		m_ambientOcclusionValues.push_back(1);
	}
}

int Chunk::getAmbientOcclusionAtVertex(glm::vec3 position, glm::vec3 normalDirection, glm::vec3 direction1, glm::vec3 direction2) {
	Block* side1 = getBlock(glm::ivec3(
		position.x + normalDirection.x + direction1.x,
		position.y + normalDirection.y + direction1.y,
		position.z + normalDirection.z + direction1.z));

	Block* side2 = getBlock(glm::ivec3(
		position.x + normalDirection.x + direction2.x,
		position.y + normalDirection.y + direction2.y,
		position.z + normalDirection.z + direction2.z));

	Block* corner = getBlock(glm::ivec3(
		position.x + normalDirection.x + direction1.x + direction2.x,
		position.y + normalDirection.y + direction1.y + direction2.y,
		position.z + normalDirection.z + direction1.z + direction2.z));

	bool side1active = false;
	bool side2active = false;
	bool corneractive = false;

	if (side1 != nullptr) side1active = side1->isActive();
	if (side2 != nullptr) side2active = side2->isActive();
	if (corner != nullptr) corneractive = corner->isActive();

	if (side1active && side2active) {
		return 0;
	}
	return 3 - (side1active + side2active + corneractive);
}

bool Chunk::blockActive(glm::ivec3 coord) {
	Block* block = getBlock(coord);

	if (block == nullptr) return false;
	return block->isActive();
}
