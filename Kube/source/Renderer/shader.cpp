#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "shader.h"
#include "../Utils/log.h"

Shader::Shader() {
	m_handle = 0;
	m_isLinked = false;
}

bool Shader::compileShader(std::string path, ShaderType shaderType) {
	GLuint shaderId;

	switch(shaderType) {
		case ShaderType::FRAGMENT:
			shaderId = glCreateShader(GL_FRAGMENT_SHADER);
			break;

		case ShaderType::VERTEX:
			shaderId = glCreateShader(GL_VERTEX_SHADER);
			break;
	} 

	// Read the shader code from file
	std::string shaderCode;
	std::ifstream shaderStream(path, std::ios::in);
	if (shaderStream.is_open()) {
		std::string Line = "";
		while (getline(shaderStream, Line))
			shaderCode += "\n" + Line;
		shaderStream.close();
	} else {
		Log::message(Log::ERROR, "Shader", "Can't find shader: " + path);
		getchar();
		return false;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile shader
	Log::message(Log::DEBUG, "Shader", "Compiling shader: " + path);
	char const * sourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	// Check shader
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> shaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderId, InfoLogLength, NULL, &shaderErrorMessage[0]);
		printf("%s\n", &shaderErrorMessage[0]);

		return false;
	}

	// Create handle if none exist
	if (m_handle == 0) {
		m_handle = glCreateProgram();
	}

	glAttachShader(m_handle, shaderId);

	return true;
}

void Shader::use() {
	if (m_isLinked)
		glUseProgram(m_handle);
}

bool Shader::link() {
	glLinkProgram(m_handle);

	GLint result = GL_FALSE;
	int infoLogLength;

	// Check the program
	glGetProgramiv(m_handle, GL_LINK_STATUS, &result);
	glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0) {
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(m_handle, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
		return false;
	}

	Log::message(Log::DEBUG, "Shader", "ProgramID: " + std::to_string(m_handle));
	m_isLinked = true;
	return true;
}

GLuint Shader::getHandle() {
	return m_handle;
}

void Shader::setUniform1f(GLchar const* location, float value1) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniform1f(locationID, value1);
}

void Shader::setUniform2f(GLchar const* location, float value1, float value2) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniform2f(locationID, value1, value2);
}

void Shader::setUniform3f(GLchar const* location, float value1, float value2, float value3) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniform3f(locationID, value1, value2, value3);
}

void Shader::setUniform4f(GLchar const* location, float value1, float value2, float value3, float value4) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniform4f(locationID, value1, value2, value3, value4);
}

void Shader::setUniform1i(GLchar const* location, int value1) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniform1i(locationID, value1);
}

void Shader::setUniform2i(GLchar const* location, int value1, int value2) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniform2i(locationID, value1, value2);
}

void Shader::setUniform3i(GLchar const* location, int value1, int value2, int value3) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniform3i(locationID, value1, value2, value3);
}

void Shader::setUniform4i(GLchar const* location, int value1, int value2, int value3, int value4) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniform4i(locationID, value1, value2, value3, value4);
}

void Shader::setUniformMatrix4fv (GLchar const* location, glm::mat4 value) {
    GLuint locationID = glGetUniformLocation(getHandle(), location);
    glUniformMatrix4fv(locationID, 1, GL_FALSE, &value[0][0]);
}
