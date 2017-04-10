# KUBE
Simple minecraft-like game and rendering engine based on OpenGL.

## Dependencies
* [GLFW](http://www.glfw.org) 3.2 or later
* [CMake](https://cmake.org) 3.0 or later
* [GLM](http://glm.g-truc.net) 0.9.8 or later
* [libnoise](http://libnoise.sourceforge.net) 1.0 or later
* OpenGL 3.0+ (2008+ Graphics Cards)

## Build instructions
### Linux
#### Installing Dependencies using APT (Ubuntu, Debian et al.)
```
apt-get install libglfw3 libglfw3-dev cmake libglm-dev libnoise-dev
```
On some older distros (Ubuntu 16.04 and below) version 3.2 of GLFW is not part of the official repositories. You should therefore build it from source. (See below)

#### Installing Dependencies using pacman (Arch Linux)
```
pacman -Syu glfw cmake glm libnoise
```

#### Installing GLFW 3.2 from source
```
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build
cd build
cmake -D BUILD_SHARED_LIBS=ON ..
sudo make install
```

#### Build KUBE
Checkout the project and make sure you have installed all dependencies.

```
mkdir build
cd build
cmake ..
make
```

It's that simple! Executables for the sample projects are now in the build folder.

### Windows
Todo
### OSX
Todo

## Features
* Phong Shading
* Ambient Occlusion
* Procedural Terrain Generation
* Dynamic Loading of Terratin
