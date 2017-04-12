#pragma once

#include <GLFW/glfw3.h>
#include <string>

//! Handles window management
class Window {
public:
  //! Default constructor
  Window();

  /*!
   * \brief Create a new window
   * \param createFullscreen True if window should be created as full-screen
   * \param windowName Name of window appearing in status bar
   * \return True if successfully created, false otherwise
   */
  bool create(bool createFullscreen, std::string windowName);

  /*!
   * \brief Updates the window each frame
   */
  void update();

  /*!
   * \brief Do rendering tasks needed on window
   */
  void render();

  int getWidth();  //!< \brief Get width of window
  int getHeight(); //!< \brief Get height of window

  int getCursorX(); //!< \brief Get current X position of cursor in window
  int getCursorY(); //!< \brief Get current Y position of cursor in window

  GLFWwindow *getWindow(); //!< \brief Get reference to GLFWwindow object

private:
  GLFWwindow *pWindow; //!< GLFWwindow object

  int m_windowHeight; //!< Window height
  int m_windowWidth;  //!< Window width

  int m_cursorX; //!< Cursor position X
  int m_cursorY; //!< Cursor position Y

  bool m_cursorLockedToCenter =
      true; //!< If true cursor gets reset to center every frame
};
