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
include_directories(include/ source/ ${GLFW_INCLUDE_DIR})

# Source files
set(KUBE_SOURCE_FILES
	source/KubeEngine.cpp
	source/KubeEngine.h
	source/KubeWindow.cpp
	source/KubeWindow.h

	source/Chunks/block.cpp
	source/Chunks/block.h
	source/Chunks/blocktype.cpp
	source/Chunks/blocktype.h
	source/Chunks/chunk.cpp
	source/Chunks/chunk.h
	source/Chunks/chunkManager.cpp
	source/Chunks/chunkManager.h

  source/Renderer/camera.cpp
  source/Renderer/camera.h
  source/Renderer/color.h
  source/Renderer/light.h
  source/Renderer/material.cpp
  source/Renderer/material.h
  source/Renderer/mesh.cpp
  source/Renderer/mesh.h
	source/Renderer/renderer.cpp
	source/Renderer/renderer.h
  source/Renderer/scenemanager.cpp
  source/Renderer/scenemanager.h
	source/Renderer/shader.cpp
	source/Renderer/shader.h
	source/Renderer/texture.cpp
	source/Renderer/texture.h
  source/Renderer/text2d.cpp
  source/Renderer/text2d.h

	source/Utils/log.cpp
	source/Utils/log.h)

add_library(Kube source/glad.c ${KUBE_SOURCE_FILES})

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
