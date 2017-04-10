#include "sample.h"
#include <iostream>
#include <Utils/log.h>
#define _USE_MATH_DEFINES
#include <math.h>

// Sorry. Quick and dirty demo.
#define COLORNUM_GRASS 185
#define WORLD_HEIGHT 16
#define MINIMUM_WORLD_HEIGHT 1
#define NOISE_FREQUENCY 0.2f
#define CAMERASPEED 30

int main(int, char**) {
	Sample app;

	app.run();

	return 0;
}

void Sample::run() {
	m_engine = new Engine();

	// Initialize Kube engine
    if (!m_engine->initialize("OpenGL Block Engine - Terrain Generation")) {
		Log::message(Log::FATAL, "Main", "Failed to initialize the engine.");
		return;
	}

	// Create a camera and move it to where we want it
	m_camera = new Camera();
	m_camera->setPerspectiveProjection(45.0f, (float)m_engine->getWindow()->getWidth() / m_engine->getWindow()->getHeight());
    m_camera->setPosition(glm::vec3(16.0f, 32.0f, 16.0f));
    m_camera->setVerticalAngle(-(float)M_PI / 5);
    m_camera->setHorizontalAngle((float)M_PI / 4);

	m_engine->setCamera(m_camera);

	m_chunkManager = new ChunkManager(3, 3, true);
    m_engine->getSceneManager()->setChunkManager(m_chunkManager);

	// Set up a basic light source (sun)
	Light *sun = new Light();
	sun->position = glm::vec4(1, 0.8, 0.6, 0);
	sun->color = glm::vec3(1, 1, 1);
	sun->ambientStrength = 0.1f;
	m_engine->getSceneManager()->addLight(sun);

	// Initialize heightmap
	heightMapBuilder.SetSourceModule(noiseGenerator);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(ChunkManager::CHUNK_SIZE, ChunkManager::CHUNK_SIZE);

	// Set chunk load callback
	m_engine->getSceneManager()->getChunkManager()->setChunkLoadCallback(std::bind(&Sample::onChunkLoad, this, std::placeholders::_1));

    while(true) {
		float deltaTime = m_engine->getDeltaTime();

		if (glfwGetKey(m_engine->getWindow()->getWindow(), GLFW_KEY_W)) {
			m_camera->moveCurrentDirection(deltaTime * CAMERASPEED);
		}

		if (glfwGetKey(m_engine->getWindow()->getWindow(), GLFW_KEY_A)) {
			m_camera->strafe(deltaTime * -CAMERASPEED);
		}

		if (glfwGetKey(m_engine->getWindow()->getWindow(), GLFW_KEY_S)) {
			m_camera->moveCurrentDirection(deltaTime * -CAMERASPEED);
		}

		if (glfwGetKey(m_engine->getWindow()->getWindow(), GLFW_KEY_D)) {
			m_camera->strafe(deltaTime * CAMERASPEED);
		}

		m_camera->rotate((m_engine->getWindow()->getWidth() / 2) - m_engine->getWindow()->getCursorX(),
			(m_engine->getWindow()->getHeight() / 2) - m_engine->getWindow()->getCursorY());

		if (!m_engine->update(deltaTime)) break;
    }
}

void Sample::onChunkLoad(Chunk* chunk) {
	// Generate heightmap for this chunk
	heightMapBuilder.SetBounds(chunk->m_chunkCoord.x * NOISE_FREQUENCY, 
		chunk->m_chunkCoord.x * NOISE_FREQUENCY + NOISE_FREQUENCY, 
		chunk->m_chunkCoord.z * NOISE_FREQUENCY, 
		chunk->m_chunkCoord.z * NOISE_FREQUENCY + NOISE_FREQUENCY);
	heightMapBuilder.Build();

	int chunkHeight = chunk->m_chunkCoord.y * ChunkManager::CHUNK_SIZE;

	for (int x = 0; x < ChunkManager::CHUNK_SIZE; x++) {
		for (int z = 0; z < ChunkManager::CHUNK_SIZE; z++) {
			float height = (heightMap.GetValue(x, z) + 1) / 2.0f;
			height = height * (WORLD_HEIGHT - MINIMUM_WORLD_HEIGHT) - MINIMUM_WORLD_HEIGHT;

			for (int y = 0; y < ChunkManager::CHUNK_SIZE; y++) {
				if (height > y + chunkHeight) {
					chunk->getBlock(glm::ivec3(x, y, z))->setColorNum(COLORNUM_GRASS);
				}
				else {
					chunk->getBlock(glm::ivec3(x, y, z))->setColorNum(0);
				}
			}
		}
	}
}
