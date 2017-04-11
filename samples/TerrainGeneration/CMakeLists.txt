set(TARGET_NAME TerrainGeneration)

find_package(LibNoise REQUIRED)

include_directories(
	../../Source
	.
	${LIBNOISE_INCLUDE_DIR}
)

add_executable(TerrainGeneration
	sample.cpp
	sample.h
	noise/noiseutils.cpp
	noise/noiseutils.h)

# Move assets to build dir
file(COPY shaders DESTINATION .)
file(COPY fonts DESTINATION .)

# Use C++11
target_compile_features(TerrainGeneration PRIVATE cxx_range_for)

target_link_libraries(TerrainGeneration Kube ${LIBNOISE_LIBRARIES})
