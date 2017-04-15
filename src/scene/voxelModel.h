#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class VoxelModel {
public:
    VoxelModel(unsigned int sizex, unsigned int sizey, unsigned int sizez);

    void setDataAtPoint(unsigned int x, unsigned int y, unsigned int z, unsigned int data);

    void mesh();

    void render();

private:
    unsigned int m_sizex;
    unsigned int m_sizey;
    unsigned int m_sizez;
    std::vector<std::vector<std::vector<unsigned int>>> m_data;

    GLuint m_VAO;
    GLuint m_vertexbuffer;
    GLuint m_colorbuffer;
    GLuint m_normalbuffer;

    int m_numberOfVertices;

    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec4> m_colors;
    std::vector<glm::vec3> m_normals;

    void meshBlock(unsigned int x, unsigned int y, unsigned int z, unsigned int data);

    void createFaceMesh(glm::vec3 normal, glm::vec4 color, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
};
