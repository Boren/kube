# Set CMake version number
cmake_minimum_required(VERSION 2.8.12)

# Set project name
project(Kube)
set(KUBE_VERSION_MAJOR 0)
set(KUBE_VERSION_MINOR 1)

# Limit the supported build configurations
set (KUBE_BUILD_CONFIGURATIONS Release Debug)

find_package(Doxygen)

option (KUBE_BUILD_SAMPLES "Build sample applications" TRUE)
option (KUBE_BUILD_DOCUMENTATION "Create HTML based engine documentation (Requires Doxygen)" ${DOXYGEN_FOUND})

add_subdirectory(Kube)
