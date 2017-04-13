#pragma once

#include "../utils/color.h"
#include <glm/glm.hpp>
#include <string>

class BlockType {
public:
  //! Default Constructor
  BlockType(std::string name, Color color);

  Color m_color;
  std::string m_name;
};
