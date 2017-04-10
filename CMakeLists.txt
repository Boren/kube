# Set CMake version number
cmake_minimum_required(VERSION 3.3.2)

# Set project name
project(Kube)
set(KUBE_VERSION_MAJOR 0)
set(KUBE_VERSION_MINOR 1)

# Limit the supported build configurations
set (KUBE_BUILD_CONFIGURATIONS Release Debug)

# Add option to build samples
option (KUBE_BUILD_SAMPLES "Build sample applications" TRUE)

add_subdirectory(Kube)
