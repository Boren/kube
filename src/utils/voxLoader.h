#pragma once

#include <string>
#include <vector>

#include <scene/voxelModel.h>

class VoxLoader {
public:
    static std::vector<VoxelModel> loadVoxFile(std::string path);
};
