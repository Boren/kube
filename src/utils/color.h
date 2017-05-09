#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <ios>
#include <sstream>
#include <iomanip>

//! Struct representation of a single color
struct Color {
    float r; //!< Red component of the color
    float g; //!< Green component of the color
    float b; //!< Blue component of the color
    float a; //!< Alpha component of the color

    /*!
     * \brief Construct a new Color with default values
     */
    Color() {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 0.0f;
    }

    /*!
     * \brief Construct a new Color with the given components
     * \param r Red component
     * \param g Green component
     * \param b Blue component
     */
    Color(float r, float g, float b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 1.0f;
    }

    /*!
     * \brief Construct a new Color with the given components
     * \param r Red component
     * \param g Green component
     * \param b Blue component
     * \param a Alpha component
     */
    Color(float r, float g, float b, float a, bool intFormat = false) {
        if (intFormat) {
            this->r = r / 255.0f;
            this->g = g / 255.0f;
            this->b = b / 255.0f;
            this->a = a / 255.0f;
        } else {
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
        }
    }

    /*!
    * \brief Construct a new Color from an unsigned int in ARGB format
    * \param color Unsigned int to construct color from
    */
    Color(unsigned int color) {
        this->r = (color % 256) / 255.0f;
        color /= 256;
        this->g = (color % 256) / 255.0f;
        color /= 256;
        this->b = (color % 256) / 255.0f;
        color /= 256;
        this->a = (color % 256) / 255.0f;
    }

    /*! Color can be implicitly converted to glm::vec4 */
    operator glm::vec4() { return glm::vec4(r, g, b, a); }

    operator std::string() const {
        std::stringstream ss;
        ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(r*255) << std::setw(2) << int(g*255) << std::setw(2) << int(b*255) << std::setw(2) << int(a*255);
        return ss.str();
    }
};