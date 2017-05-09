#include "sample.h"

#include <utils/log.h>
#include <utils/voxLoader.h>

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
    m_camera->setPerspectiveProjection(65.0f,
                                       (float) m_engine->getWindow()->getWidth() /
                                       m_engine->getWindow()->getHeight());

    m_engine->setCamera(m_camera);

    // Set up a basic light source (sun)
    Light *sun = new Light();
    sun->position = glm::vec4(1, 0.8, -0.6, 0);
    sun->color = glm::vec3(1.0f, 1.0f, 1.0f);
    sun->ambientStrength = 1.0f;
    m_engine->getSceneManager()->addLight(sun);

    // Set up a test point light
    Light *pointLight = new Light();
    pointLight->position = glm::vec4(20.0f, 20.0f, 30.0f, 1);
    pointLight->color = glm::vec3(1.0f, 0.58f, 0.16f);
    pointLight->ambientStrength = 0.0f;
    pointLight->attenuation = 0.0f;
    m_engine->getSceneManager()->addLight(pointLight);

    VoxelModel m = VoxLoader::loadVoxFile("assets/models/monu1.vox")[0];
    m_engine->getSceneManager()->addVoxelModel(&m);

    /*VoxelModel torch = VoxLoader::loadVoxFile("assets/models/torch.vox");
    torch.setPosition(glm::vec3(20.0f, 20.0f, 0.0f));
    m_engine->getSceneManager()->addVoxelModel(&torch);

    VoxelModel pikachu = VoxLoader::loadVoxFile("assets/models/pikachu.vox");
    pikachu.setPosition(glm::vec3(-20.0f, 10.0f, 0.0f));
    m_engine->getSceneManager()->addVoxelModel(&pikachu);*/

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

        if (m_engine->getInputManager()->keyPressed(GLFW_KEY_SPACE)) {
            toggleRenderMode();
        }

        GLfloat xoffset = (m_engine->getWindow()->getWidth() / 2) - m_engine->getWindow()->getCursorX();
        GLfloat yoffset = (m_engine->getWindow()->getHeight() / 2) - m_engine->getWindow()->getCursorY();

        GLfloat sensitivity = 0.05;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        m_camera->setYaw(m_camera->getYaw() + xoffset);
        m_camera->setPitch(m_camera->getPitch() + yoffset);

        glm::vec3 pos = m_camera->getPosition();
        pos.z = 10.0f;
        m_camera->setPosition(pos);


        if (!m_engine->update(deltaTime))
            break;
    }
}

void Sample::toggleRenderMode() {
    if (m_engine->getRenderer()->getCurrentRenderMode() ==
        RenderMode::RENDERMODE_SHADED) {
        m_engine->getRenderer()->setRenderMode(RenderMode::RENDERMODE_WIREFRAME);
    } else {
        m_engine->getRenderer()->setRenderMode(RenderMode::RENDERMODE_SHADED);
    }
}
