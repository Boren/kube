#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.h"

class TextManager {
public:
  TextManager();
  // TODO: Add ability to add more fonts
  // void addFont();
  void renderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
  Shader m_textShader;
  FT_Face face;
  FT_Library ft;
};
