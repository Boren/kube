find_package(LibNoise REQUIRED)

include_directories(
	../../Source
	.
	${LIBNOISE_INCLUDE}
)

add_executable(PointLightTest
	sample.cpp
	sample.h)

# Use C++11
target_compile_features(PointLightTest PRIVATE cxx_range_for)

target_link_libraries(PointLightTest Kube ${LIBNOISE_LIBRARY})
