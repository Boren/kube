#include "voxLoader.h"
#include "log.h"

#include <iostream>
#include <sstream>
#include <string.h>

// TODO: Add support for PACK and split functionality into functions
VoxelModel VoxLoader::loadVoxFile(std::string path) {
    Log::message(Log::DEBUG, "VoxLoader", "Loading vox file from " + path);

    FILE *fp;

    // Open file
    fp = fopen(path.c_str(), "rb");
    if (fp == NULL) {
        Log::message(Log::ERROR, "VoxLoader", "Can't find vox file: " + path);
        throw;
    }

    // Verify Filetype
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "VOX ", 4) != 0) {
        fclose(fp);
        Log::message(Log::ERROR, "VoxLoader", "File format invalid or corrupted");
        throw;
    }

    // Version Number
    int version[1];
    fread((void *) version, sizeof(int), 1, fp);
    Log::message(Log::DEBUG, "VoxLoader", "Version: " + std::to_string(version[0]));

    // Read MAIN chunk
    char chunkname[4];
    fread(chunkname, 1, 4, fp);
    if (strncmp(chunkname, "MAIN ", 4) != 0) {
        fclose(fp);
        Log::message(Log::ERROR, "VoxLoader", "Expected MAIN chunk");
        throw;
    }

    int chunkSize[1];
    int chunkChildren[1];
    fread((void *) chunkSize, sizeof(int), 1, fp);
    fread((void *) chunkChildren, sizeof(int), 1, fp);

    Log::message(Log::DEBUG, "VoxLoader", "--" + std::string(chunkname) + " Chunk --");
    Log::message(Log::DEBUG, "VoxLoader", "Chunk size: " + std::to_string(chunkSize[0]));
    Log::message(Log::DEBUG, "VoxLoader", "Children size: " + std::to_string(chunkChildren[0]));

    // Read SIZE chunk
    fread(chunkname, 1, 4, fp);
    if (strncmp(chunkname, "SIZE ", 4) != 0) {
        fclose(fp);
        Log::message(Log::ERROR, "VoxLoader", "Expected SIZE chunk");
        throw;
    }

    fread((void *) chunkSize, sizeof(int), 1, fp);
    fread((void *) chunkChildren, sizeof(int), 1, fp);

    Log::message(Log::DEBUG, "VoxLoader", "--" + std::string(chunkname) + " Chunk --");
    Log::message(Log::DEBUG, "VoxLoader", "Chunk size: " + std::to_string(chunkSize[0]));
    Log::message(Log::DEBUG, "VoxLoader", "Children size: " + std::to_string(chunkChildren[0]));

    // Model size
    unsigned int sizex[1];
    unsigned int sizey[1];
    unsigned int sizez[1];
    fread((void *) sizex, sizeof(int), 1, fp);
    fread((void *) sizey, sizeof(int), 1, fp);
    fread((void *) sizez, sizeof(int), 1, fp);

    Log::message(Log::DEBUG, "VoxLoader", "Size X: " + std::to_string(sizex[0]));
    Log::message(Log::DEBUG, "VoxLoader", "Size Y: " + std::to_string(sizey[0]));
    Log::message(Log::DEBUG, "VoxLoader", "Size Z: " + std::to_string(sizez[0]));

    // Read XYZI chunk
    fread(chunkname, 1, 4, fp);
    if (strncmp(chunkname, "XYZI ", 4) != 0) {
        fclose(fp);
        Log::message(Log::ERROR, "VoxLoader", "Expected XYZI chunk");
        throw;
    }

    fread((void *) chunkSize, sizeof(int), 1, fp);
    fread((void *) chunkChildren, sizeof(int), 1, fp);

    Log::message(Log::DEBUG, "VoxLoader", "--" + std::string(chunkname) + " Chunk --");
    Log::message(Log::DEBUG, "VoxLoader", "Chunk size: " + std::to_string(chunkSize[0]));
    Log::message(Log::DEBUG, "VoxLoader", "Children size: " + std::to_string(chunkChildren[0]));

    // Data
    int numvoxels[1];
    fread((void *) numvoxels, sizeof(int), 1, fp);

    Log::message(Log::DEBUG, "VoxLoader", "Number of voxels: " + std::to_string(numvoxels[0]));

    std::vector<std::vector<std::vector<unsigned int>>> data;

    VoxelModel model(sizex[0], sizey[0], sizez[0]);

    unsigned char x[1];
    unsigned char y[1];
    unsigned char z[1];
    unsigned char c[1];

    for (int i = 0; i < numvoxels[0]; i = i + 1) {
        fread(x, 1, 1, fp);
        fread(y, 1, 1, fp);
        fread(z, 1, 1, fp);
        fread(c, 1, 1, fp);
        model.setDataAtPoint(x[0], y[0], z[0], c[0]);
    }

    model.mesh();

    Log::message(Log::DEBUG, "VoxLoader", "Finished!");
    return model;
}