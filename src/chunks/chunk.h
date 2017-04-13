#pragma once

#include "block.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

//! \brief Single chunk in the world
//! \todo Re-implement ambient occlusion
//! \todo Move part of code to mesh class
class Chunk {
public:
  /*!
   * \brief Default constructor
   * \param chunkCoord Coordinate of chunk
   */
  Chunk(glm::ivec3 chunkCoord);

  /*!
   * \brief Load the chunk
   */
  void load();

  /*!
  * \brief Creates a mesh of the chunk
  */
  void mesh();

  /*!
   * \brief Update chunk
   * \param deltaTime Time passed since last update
   * \remark This function is automatically called each frame by the engine.
   */
  void update(float deltaTime);

  /*!
   * \brief Renders the chunk
   */
  void render();

  /*!
   * \brief Check if chunk is loaded
   * \return True if loaded, false otherwise
   */
  bool isLoaded();

  /*!
  * \brief Check if chunk is meshed
  * \return True if meshed, false otherwise
  */
  bool isMeshed();

  /*!
   * \brief Get block at specified coordinate
   * \param coord Coordinate to get block at
   * \return Block at coord
   */
  Block *getBlock(glm::ivec3 coord);

  static const float BLOCK_SIZE; //!< Draw size of a single block

  int m_numberOfVertices;

  glm::ivec3 m_chunkCoord; //!< Coordinate of this chunk in chunkmanager

  Chunk *m_left;   //!< Reference to adjacent chunk to the left
  Chunk *m_right;  //!< Reference to adjacent chunk to the right
  Chunk *m_top;    //!< Reference to adjacent chunk above
  Chunk *m_bottom; //!< Reference to adjacent chunk below
  Chunk *m_front;  //!< Reference to adjacent chunk in front
  Chunk *m_back;   //!< Reference to adjacent chunk behind

private:
  /*!
   * \brief Builds a mesh of the current chunk
   */
  void buildMesh();

  /*!
   * \brief Helper function to create a mesh of a block
   * \param block The block to mesh
   * \param blockCoord Coordinate of the block to mesh
   */
  void createBlockMesh(Block *block, glm::ivec3 blockCoord,
                       glm::ivec3 blockCoordLocal);

  /*!
   * \brief Helper function to create a mesh of a block face
   * \param normal Normal vector to this face
   * \param p1 First point of the face
   * \param p2 Second point of the face
   * \param p3 Third point of the face
   * \param p4 Fourth point of the face
   * \param color Color of face
   */
  void createFaceMesh(glm::vec3 normal, glm::vec3 p1, glm::vec3 p2,
                      glm::vec3 p3, glm::vec3 p4, glm::vec4 color,
                      glm::ivec3 blockCoordLocal);

  int getAmbientOcclusionAtVertex(glm::vec3 position, glm::vec3 normalDirection,
                                  glm::vec3 direction1, glm::vec3 direction2);

  /*!
   * \brief Get active state of a block
   * \param coord Coordinate of block to get active state of
   * \return True if block at coord is active, false otherwise
   */
  bool blockActive(glm::ivec3 coord);

  bool m_isLoaded = false; //!< True if chunk is loaded
  bool m_isMeshed = false; //!< True if chunk is meshed

  Block *m_blocks; //!< Block information storage

  GLuint m_VAO;
  GLuint m_vertexbuffer;
  GLuint m_colorbuffer;
  GLuint m_normalbuffer;
  GLuint m_ambientOcclusionBuffer;

  std::vector<glm::vec3> m_vertices;
  std::vector<glm::vec4> m_colors;
  std::vector<glm::vec3> m_normals;
  std::vector<float> m_ambientOcclusionValues;
};
