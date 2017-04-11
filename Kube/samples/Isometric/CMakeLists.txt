set(TARGET_NAME Isometric)

include_directories(
	../../Source
	.
)

add_executable(Isometric
	sample.cpp
	sample.h)

# Move assets to build dir
file(COPY shaders DESTINATION .)
file(COPY fonts DESTINATION .)

# Use C++11
target_compile_features(Isometric PRIVATE cxx_range_for)

target_link_libraries(Isometric Kube)
