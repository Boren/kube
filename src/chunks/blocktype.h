#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../utils/color.h"

class BlockType {
public:
    //! Default Constructor
    BlockType(std::string name, Color color);

    Color m_color;
    std::string m_name;
};
