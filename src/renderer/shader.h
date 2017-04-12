#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

//! \brief Holds a compiled shader and simplifies referencing them
//! \todo Improve support for uniform variables by querying shader and
//! overloading method
//! \todo Separate file IO
class Shader {
public:
  //! \enum Shader Type
  /*!
    Defines what type of shader is to be used.
  */
  enum ShaderType {
    VERTEX,  //!< Vertex Shader
    FRAGMENT //!< Fragment Shader
  };

  //! Default Constructor
  Shader();

  /*!
   * \brief Compiles a shader
   * \param path File path from current working directory containing the shader
   * source
   * \param shaderType The type of shader to be compiled
   * \return True if shader compiled successfully, false otherwise
   */
  bool compileShader(std::string path, ShaderType shaderType);

  /*!
   * \brief Links the combined shader consisting of the compiled shaders
   * \return True if shader linked successfully, false otherwise
   */
  bool link();

  /*!
   * \brief Use this shader for subsequent render calls
   */
  void use();

  /*!
   * \brief Gets a handle to this shader
   * \return GLuint designating program ID
   */
  GLuint getHandle();

  /*!
   * \brief Sets a uniform variable of type 1f
   * \param location String representation of variable to set
   * \param value Value to set uniform variable to
   */
  void setUniform1f(GLchar const *location, float value);

  /*!
   * \brief Sets a uniform variable of type 2f
   * \param location String representation of variable to set
   * \param value Value to set uniform variable to
   */
  void setUniform2f(GLchar const *location, float value1, float value2);

  /*!
   * \brief Sets a uniform variable of type 3f
   * \param location String representation of variable to set
   * \param value Value to set uniform variable to
   */
  void setUniform3f(GLchar const *location, float value1, float value2,
                    float value3);

  /*!
   * \brief Sets a uniform variable of type 4f
   * \param location String representation of variable to set
   * \param value Value to set uniform variable to
   */
  void setUniform4f(GLchar const *location, float value1, float value2,
                    float value3, float value4);

  /*!
   * \brief Sets a uniform variable of type 1i
   * \param location String representation of variable to set
   * \param value Value to set uniform variable to
   */
  void setUniform1i(GLchar const *location, int value1);

  /*!
  * \brief Sets a uniform variable of type 2i
  * \param location String representation of variable to set
  * \param value Value to set uniform variable to
  */
  void setUniform2i(GLchar const *location, int value1, int value2);

  /*!
  * \brief Sets a uniform variable of type 3i
  * \param location String representation of variable to set
  * \param value Value to set uniform variable to
  */
  void setUniform3i(GLchar const *location, int value1, int value2, int value3);

  /*!
  * \brief Sets a uniform variable of type 4i
  * \param location String representation of variable to set
  * \param value Value to set uniform variable to
  */
  void setUniform4i(GLchar const *location, int value1, int value2, int value3,
                    int value4);

  /*!
  * \brief Sets a uniform variable of type Float Matrix 4x4
  * \param location String representation of variable to set
  * \param value Value to set uniform variable to
  */
  void setUniformMatrix4fv(GLchar const *location, glm::mat4 value);

private:
  bool m_isLinked; //!< True if linked
  GLuint m_handle; //!< Reference to program ID
};
