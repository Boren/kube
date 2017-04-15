#pragma once

#include <glm/glm.hpp>

#include "../utils/color.h"

//! Represents a single instance of light in the world
struct Light {
    glm::vec4 position;
    glm::vec3 color;
    float attenuation;
    float ambientStrength;
};
