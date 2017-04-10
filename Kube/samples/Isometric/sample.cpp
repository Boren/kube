#include "sample.h"
#include <iostream>
#include <Utils/log.h>
#define _USE_MATH_DEFINES
#include <math.h>



int main(int, char**) {
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
	m_camera->setPerspectiveProjection(45.0f, (float)m_engine->getWindow()->getWidth() / m_engine->getWindow()->getHeight());
	m_camera->setPosition(glm::vec3(-32.0f, 70.0f, 16.0f));
	m_camera->setVerticalAngle(-(float)M_PI / 3.5f);
	m_camera->setHorizontalAngle((float)M_PI / 2.0f);

	m_engine->setCamera(m_camera);

	m_chunkManager = new ChunkManager(0, 0, false);
	m_engine->getSceneManager()->setChunkManager(m_chunkManager);

	// Set up a basic light source (sun)
	Light *sun = new Light();
	sun->position = glm::vec4(1, 0.8, -0.2, 0);
	sun->color = glm::vec3(1, 0, 0);
	sun->ambientStrength = 0.2f;
	m_engine->getSceneManager()->addLight(sun);

	// Set up a test point light
	Light *pointLight = new Light();
	pointLight->position = glm::vec4(-20, 55, -38, 1);
	pointLight->color = glm::vec3(1, 0, 0);
	pointLight->ambientStrength = 0.1f;
	pointLight->attenuation = 0.0f;
	//m_engine->getSceneManager()->addLight(pointLight);

	m_engine->getSceneManager()->getChunkManager()->setChunkLoadCallback(std::bind(&Sample::onChunkLoad, this, std::placeholders::_1));

	while (true) {
		float deltaTime = m_engine->getDeltaTime();
		if (!m_engine->update(deltaTime)) break;
	}
}

void Sample::onChunkLoad(Chunk* chunk) {
	for (int x = 0; x < ChunkManager::CHUNK_SIZE; x++) {
		for (int z = 0; z < ChunkManager::CHUNK_SIZE; z++) {
			for (int y = 0; y < ChunkManager::CHUNK_SIZE; y++) {
chunk->getBlock(glm::ivec3(x, y, z))->setColorNum(1);
			}
		}
	}
}
