#include "voxelModel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <utils/log.h>

VoxelModel::VoxelModel(unsigned int sizex, unsigned int sizey, unsigned int sizez) {
    m_sizex = sizex;
    m_sizey = sizey;
    m_sizez = sizez;

    m_data.resize(sizex);
    for (unsigned int i = 0; i < sizex; ++i) {
        m_data[i].resize(sizey);

        for (unsigned int j = 0; j < sizey; ++j) {
            m_data[i][j].resize(sizez);

            for (unsigned int k = 0; k < sizez; ++k) {
                m_data[i][j][k] = 0;
            }
        }
    }

    m_position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void VoxelModel::setPosition(glm::vec3 newPosition) {
    m_position = newPosition;
}

void VoxelModel::setDataAtPoint(unsigned int x, unsigned int y, unsigned int z, unsigned int data) {
    m_data[x][y][z] = data;
}

void VoxelModel::mesh() {
    Log::message(Log::DEBUG, "VoxelModel", "Meshing model");

    for (unsigned int x = 0; x < m_sizex; x++) {
        for (unsigned int y = 0; y < m_sizey; y++) {
            for (unsigned int z = 0; z < m_sizez; z++) {
                if (m_data[x][y][z] != 0) {
                    meshBlock(x, y, z, m_data[x][y][z]);
                }
            }
        }
    }

    m_numberOfVertices = (int) m_vertices.size();

    if (m_numberOfVertices != 0) {
        glGenBuffers(1, &m_vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3),
                     &m_vertices[0], GL_STATIC_DRAW);
        m_vertices.clear();

        glGenBuffers(1, &m_colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4),
                     &m_colors[0], GL_STATIC_DRAW);
        m_colors.clear();

        glGenBuffers(1, &m_normalbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
        glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3),
                     &m_normals[0], GL_STATIC_DRAW);
        m_normals.clear();
    }

    glCreateVertexArrays(1, &m_VAO);

    // Enable attributes
    glEnableVertexArrayAttrib(m_VAO, 0);
    glEnableVertexArrayAttrib(m_VAO, 1);
    glEnableVertexArrayAttrib(m_VAO, 2);

    // Specify format
    glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_VAO, 1, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(m_VAO, 2, 3, GL_FLOAT, GL_FALSE, 0);

    // Bind attributes to buffers
    glVertexArrayAttribBinding(m_VAO, 0, 0);
    glVertexArrayAttribBinding(m_VAO, 1, 1);
    glVertexArrayAttribBinding(m_VAO, 2, 2);

    // Buffer data to vertex array
    glVertexArrayVertexBuffer(m_VAO, 0, m_vertexbuffer, 0, 3 * sizeof(float));
    glVertexArrayVertexBuffer(m_VAO, 1, m_colorbuffer, 0, 4 * sizeof(float));
    glVertexArrayVertexBuffer(m_VAO, 2, m_normalbuffer, 0, 3 * sizeof(float));
}

void VoxelModel::render(Shader *shader) {
    glm::mat4 transform;
    transform = glm::translate(transform, m_position);
    GLint transformLoc = glGetUniformLocation(shader->getHandle(), "model");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(m_VAO);

    // ... and DRAW!
    glDrawArrays(GL_TRIANGLES, 0, m_numberOfVertices);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

void VoxelModel::meshBlock(unsigned int x, unsigned int y, unsigned int z, unsigned int data) {
    glm::vec4 color = getColor(data);

    glm::vec3 p1(x + 0, y + 0, z + 1);
    glm::vec3 p2(x + 1, y + 0, z + 1);
    glm::vec3 p3(x + 1, y + 1, z + 1);
    glm::vec3 p4(x + 0, y + 1, z + 1);
    glm::vec3 p5(x + 1, y + 0, z + 0);
    glm::vec3 p6(x + 0, y + 0, z + 0);
    glm::vec3 p7(x + 0, y + 1, z + 0);
    glm::vec3 p8(x + 1, y + 1, z + 0);

    createFaceMesh(glm::vec3(1, 0, 0), color, p2, p5, p8, p3);
    createFaceMesh(glm::vec3(-1, 0, 0), color, p6, p1, p4, p7);
    createFaceMesh(glm::vec3(0, 1, 0), color, p4, p3, p8, p7);
    createFaceMesh(glm::vec3(0, -1, 0), color, p6, p5, p2, p1);
    createFaceMesh(glm::vec3(0, 0, 1), color, p1, p2, p3, p4);
    createFaceMesh(glm::vec3(0, 0, -1), color, p5, p6, p7, p8);
}

void VoxelModel::createFaceMesh(glm::vec3 normal, glm::vec4 color,
                                glm::vec3 p0, glm::vec3 p1,
                                glm::vec3 p2, glm::vec3 p3) {
    m_vertices.push_back(p0);
    m_vertices.push_back(p1);
    m_vertices.push_back(p2);

    m_vertices.push_back(p0);
    m_vertices.push_back(p2);
    m_vertices.push_back(p3);

    // Normals
    for (int i = 0; i < 6; i++) {
        m_normals.push_back(normal);
    }

    // Colors
    for (int i = 0; i < 6; i++) {
        m_colors.push_back(color);
    }
}

Color VoxelModel::getColor(unsigned int index) {
    if (_useCustomColorPalette) {
        return Color(_colorPalette[index]);
    } else {
        return Color(defaultColorPalette[index]);
    }
}

void VoxelModel::setColorPalette(std::vector<unsigned int> colorPalette) {
    _colorPalette = colorPalette;
    _useCustomColorPalette = true;
}
