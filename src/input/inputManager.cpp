#include "inputManager.h"

#include <sstream>

#include "../utils/log.h"

std::unordered_set<int> InputManager::pressedKeys;
// std::unordered_set<int> InputManager::pressedScancodes;
std::unordered_set<int> InputManager::pressedButtons;
std::unordered_set<int> InputManager::newPressedKeys;
// std::unordered_set<int> InputManager::newPressedScancodes;
std::unordered_set<int> InputManager::newPressedButtons;

InputManager::InputManager(GLFWwindow *window) {
        mWindow = window;
        glfwSetKeyCallback(mWindow, keyCallback);
        glfwSetInputMode(mWindow, GLFW_STICKY_KEYS, 0);
}

void InputManager::update() {
        // Update pressed keys with the keys pressed in the last frame and start again
        pressedKeys.clear();
        pressedKeys.insert(newPressedKeys.begin(), newPressedKeys.end());
        newPressedKeys.clear();
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if(action == GLFW_REPEAT)
                return;

        std::stringstream ss;
        std::string state = "";

        if(action == GLFW_PRESS) {
                state = "pressed";
        } else if (action == GLFW_RELEASE) {
                state = "released";
        }

        ss << "Key " << key << " (Scandode: " << scancode << ") was " << state;
        Log::message(Log::DEBUG, "Input", ss.str());

        if(key != GLFW_KEY_UNKNOWN) {
                if(action == GLFW_PRESS) {
                        newPressedKeys.insert(key);
                }
        }

        /*
        if(action == GLFW_PRESS) {
                newPressedScancodes.insert(scancode);
        }
        */
}

bool InputManager::keyHeld(int key) {
        return glfwGetKey(mWindow, key);
}

bool InputManager::mouseButtonHeld(int button) {
        return glfwGetMouseButton(mWindow, button);
}

bool InputManager::keyPressed(int key) {
        return pressedKeys.find(key) != pressedKeys.end();
}

bool InputManager::mouseButtonPressed(int button) {
        return pressedButtons.find(button) != pressedButtons.end();
}
