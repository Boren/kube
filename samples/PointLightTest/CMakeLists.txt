set(TARGET_NAME PointLightTest)

include_directories(
	../../Source
	.
)

add_executable(PointLightTest
	sample.cpp
	sample.h)

# Move assets to build dir
file(COPY assets DESTINATION .)

# Use C++11
target_compile_features(PointLightTest PRIVATE cxx_range_for)

target_link_libraries(PointLightTest Kube)
