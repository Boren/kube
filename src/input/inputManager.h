#pragma once

#include <unordered_set>
#include <vector>

#include <GLFW/glfw3.h>

class InputManager {
public:
  InputManager(GLFWwindow *window);
  void update();
  bool keyPressed(int key);
  bool keyHeld(int key);
  // TODO: Released keys
  // bool keyReleased(int key);
  // TODO: Scancodes
  // bool scancodePressed(int scancode);
  // bool scancodeHeld(int scancode);
  // bool scancodeReleased(int scancode);
  bool mouseButtonPressed(int button);
  bool mouseButtonHeld(int button);
  // TODO: Release mouse buttons
  // bool mouseButtonReleased(int button);

private:
  GLFWwindow *mWindow;
  static std::unordered_set<int> pressedKeys;
  // static std::unordered_set<int> pressedScancodes;
  static std::unordered_set<int> pressedButtons;
  static std::unordered_set<int> newPressedKeys;
  // static std::unordered_set<int> newPressedScancodes;
  static std::unordered_set<int> newPressedButtons;

  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
};
