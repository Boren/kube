#pragma once

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include "shader.h"

//! Generic mesh object containing information about how to render complex
//! geometric shapes
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    /*  Functions  */
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);

    void Draw(Shader shader);

private:
    GLuint VAO, VBO, EBO;

    void setupMesh();
};
