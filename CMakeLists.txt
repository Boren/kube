# Set CMake version number
cmake_minimum_required(VERSION 2.8.12)

# Set project name and version
project(Kube)
set(KUBE_VERSION_MAJOR 0)
set(KUBE_VERSION_MINOR 1)

# Limit the supported build configurations
set (KUBE_BUILD_CONFIGURATIONS Release Debug)

find_package(Doxygen)

option (KUBE_BUILD_SAMPLES "Build sample applications" TRUE)
option (KUBE_BUILD_DOCUMENTATION "Create HTML based engine documentation (Requires Doxygen)" ${DOXYGEN_FOUND})

# Define target name
set (TARGET_NAME Kube)

# Flags
# TODO: Figure out why we need this, GLFW is not happy without
add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-D_CRT_SECURE_NO_WARNINGS
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# GLFW
find_package(GLFW 3.2 REQUIRED)
include_directories(include/ src/ ${GLFW_INCLUDE_DIR})

# Source files
set(KUBE_SOURCE_FILES
	src/kubeEngine.cpp
	src/kubeEngine.h
	src/kubeWindow.cpp
	src/kubeWindow.h

	src/chunks/block.cpp
	src/chunks/block.h
	src/chunks/blocktype.cpp
	src/chunks/blocktype.h
	src/chunks/chunk.cpp
	src/chunks/chunk.h
	src/chunks/chunkManager.cpp
	src/chunks/chunkManager.h

	src/input/inputManager.cpp
	src/input/inputManager.h

  src/renderer/camera.cpp
  src/renderer/camera.h
  src/renderer/color.h
  src/renderer/light.h
  src/renderer/material.cpp
  src/renderer/material.h
  src/renderer/mesh.cpp
  src/renderer/mesh.h
	src/renderer/renderer.cpp
	src/renderer/renderer.h
  src/renderer/scenemanager.cpp
  src/renderer/scenemanager.h
	src/renderer/shader.cpp
	src/renderer/shader.h
	src/renderer/texture.cpp
	src/renderer/texture.h
  src/renderer/text2d.cpp
  src/renderer/text2d.h

	src/utils/log.cpp
  src/utils/log.h)

add_library(Kube src/glad.c ${KUBE_SOURCE_FILES})

# Use C++11
target_compile_features(Kube PRIVATE cxx_range_for)

target_link_libraries(Kube ${GLFW_LIBRARIES})


IF(KUBE_BUILD_SAMPLES)
	add_subdirectory(samples)
ENDIF()

IF(KUBE_BUILD_DOCUMENTATION)
	if(NOT DOXYGEN_FOUND)
			message(FATAL_ERROR "Doxygen is needed to build the documentation.")
	endif()

	set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
	set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	configure_file(${doxyfile_in} ${doxyfile} @ONLY)

	add_custom_target(doc
			COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM)

	#install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
ENDIF()
