#include "sample.h"
#include <utils/log.h>
#include <utils/voxLoader.h>

#define _USE_MATH_DEFINES

#define CAMERASPEED 30

int main(int, char **) {
    Sample app;

    app.run();

    return 0;
}

void Sample::run() {
    m_engine = new Engine();

    // Initialize Kube engine
    if (!m_engine->initialize("OpenGL Block Engine - Vox Loading")) {
        Log::message(Log::FATAL, "Main", "Failed to initialize the engine.");
        return;
    }

    // Create a camera and move it to where we want it
    m_camera = new Camera();
    m_camera->setPerspectiveProjection(45.0f,
                                       (float) m_engine->getWindow()->getWidth() /
                                       m_engine->getWindow()->getHeight());
    m_camera->setPosition(glm::vec3(10.0f, -20.0f, 10.0f));
    m_camera->setVerticalAngle((float) M_PI / 2.0f);
    m_camera->setHorizontalAngle((float) M_PI);

    m_engine->setCamera(m_camera);

    // Set up a basic light source (sun)
    Light *sun = new Light();
    sun->position = glm::vec4(1, 0.8, -0.6, 0);
    sun->color = glm::vec3(1, 1, 1);
    sun->ambientStrength = 1.0f;
    m_engine->getSceneManager()->addLight(sun);

    VoxelModel m = VoxLoader::loadVoxFile("assets/models/chr_man.vox");
    m_engine->getSceneManager()->addVoxelModel(&m);

    while (true) {
        float deltaTime = m_engine->getDeltaTime();

        if (m_engine->getInputManager()->keyHeld(GLFW_KEY_W)) {
            m_camera->moveCurrentDirection(deltaTime * CAMERASPEED);
        }

        if (m_engine->getInputManager()->keyHeld(GLFW_KEY_A)) {
            m_camera->strafe(deltaTime * -CAMERASPEED);
        }

        if (m_engine->getInputManager()->keyHeld(GLFW_KEY_S)) {
            m_camera->moveCurrentDirection(deltaTime * -CAMERASPEED);
        }

        if (m_engine->getInputManager()->keyHeld(GLFW_KEY_D)) {
            m_camera->strafe(deltaTime * CAMERASPEED);
        }

        m_camera->rotate((m_engine->getWindow()->getWidth() / 2) -
                         m_engine->getWindow()->getCursorX(),
                         (m_engine->getWindow()->getHeight() / 2) -
                         m_engine->getWindow()->getCursorY());

        if (!m_engine->update(deltaTime))
            break;
    }
}
