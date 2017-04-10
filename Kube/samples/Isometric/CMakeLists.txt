find_package(LibNoise REQUIRED)

include_directories(
	../../Source
	.
	${LIBNOISE_INCLUDE}
)

add_executable(Isometric
	sample.cpp
	sample.h)

# Use C++11
target_compile_features(Isometric PRIVATE cxx_range_for)

target_link_libraries(Isometric Kube ${LIBNOISE_LIBRARY})
