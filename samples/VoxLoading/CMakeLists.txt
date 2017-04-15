set(TARGET_NAME VoxLoading)

include_directories(
        ../../Source
        .
)

add_executable(VoxLoading
        sample.cpp
        sample.h)

# Move assets to build dir
file(COPY assets DESTINATION .)

# Use C++11
target_compile_features(VoxLoading PRIVATE cxx_range_for)

target_link_libraries(VoxLoading Kube)
