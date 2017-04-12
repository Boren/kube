#pragma once

#include <glm/glm.hpp>
#include "../renderer/color.h"
#include <string>

class BlockType {
public:
    //! Default Constructor
    BlockType(std::string name, Color color);

    Color m_color;
	std::string m_name;
};
