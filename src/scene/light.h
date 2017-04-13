#pragma once

#include "../utils/color.h"
#include <glm/glm.hpp>

//! Represents a single instance of light in the world
struct Light {
  glm::vec4 position;
  glm::vec3 color;
  float attenuation;
  float ambientStrength;
};
