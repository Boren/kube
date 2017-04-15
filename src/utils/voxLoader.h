#ifndef KUBE_VOXLOADER_H
#define KUBE_VOXLOADER_H

#include <string>
#include <vector>
#include <scene/voxelModel.h>

class VoxLoader {
public:
    static VoxelModel loadVoxFile(std::string path);
};


#endif //KUBE_VOXLOADER_H
