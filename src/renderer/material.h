#pragma once

#include "../utils/color.h"

//! Represents a material which can be applied to different objects
class Material {
public:
private:
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    float shininess;
};
