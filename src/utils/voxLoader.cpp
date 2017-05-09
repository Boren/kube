#include "voxLoader.h"

#include <string.h>
#include <fstream>

#include "log.h"


#define VOX_FILE_HEADER "VOX "

// TODO: Add support for PACK and split functionality into functions
std::vector<VoxelModel> VoxLoader::loadVoxFile(std::string path) {
    Log::message(Log::DEBUG, "VoxLoader", "Loading vox file from " + path);

    // Open file
    std::ifstream input(path.c_str(), std::ifstream::binary);
    if (!input) {
        Log::message(Log::ERROR, "VoxLoader", "Can't find vox file: " + path);
        throw;
    }

    // Verify Filetype
    char filecode[4];
    input.read(filecode, 4);
    if (strncmp(filecode, VOX_FILE_HEADER, 4) != 0) {
        Log::message(Log::ERROR, "VoxLoader", "File format invalid or corrupted");
        input.close();
        throw;
    }

    // Version Number
    int version;
    input.read(reinterpret_cast<char*>(&version), sizeof(int));
    Log::message(Log::DEBUG, "VoxLoader", "Version: " + std::to_string(version));

    // Read MAIN chunk
    char chunkname[4];
    input.read(chunkname, 4);
    if (strncmp(chunkname, "MAIN ", 4) != 0) {
        Log::message(Log::ERROR, "VoxLoader", "Expected MAIN chunk");
        input.close();
        throw;
    }

    int chunkSize;
    int chunkChildren;
    input.read(reinterpret_cast<char*>(&chunkSize), sizeof(int));
    input.read(reinterpret_cast<char*>(&chunkChildren), sizeof(int));

    Log::message(Log::DEBUG, "VoxLoader", "--" + std::string(chunkname) + " Chunk --");
    Log::message(Log::DEBUG, "VoxLoader", "Chunk size: " + std::to_string(chunkSize));
    Log::message(Log::DEBUG, "VoxLoader", "Children size: " + std::to_string(chunkChildren));


    /* START MAIN VOXEL PARSING */

    std::vector<VoxelModel> models;
    bool customColorPalette = false;
    std::vector<unsigned int> colorPalette;

    while(!input.eof()) {
        // Read chunk name
        input.read(chunkname, 4);
        input.read(reinterpret_cast<char*>(&chunkSize), sizeof(int));
        input.read(reinterpret_cast<char*>(&chunkChildren), sizeof(int));

        Log::message(Log::DEBUG, "VoxLoader", "--" + std::string(chunkname) + " Chunk --");
        Log::message(Log::DEBUG, "VoxLoader", "Chunk size: " + std::to_string(chunkSize));
        Log::message(Log::DEBUG, "VoxLoader", "Children size: " + std::to_string(chunkChildren));

        if (strncmp(chunkname, "SIZE ", 4) == 0) {
            // Model size
            unsigned int sizex;
            unsigned int sizey;
            unsigned int sizez;
            input.read(reinterpret_cast<char *>(&sizex), sizeof(int));
            input.read(reinterpret_cast<char *>(&sizey), sizeof(int));
            input.read(reinterpret_cast<char *>(&sizez), sizeof(int));

            Log::message(Log::DEBUG, "VoxLoader", "Size X: " + std::to_string(sizex));
            Log::message(Log::DEBUG, "VoxLoader", "Size Y: " + std::to_string(sizey));
            Log::message(Log::DEBUG, "VoxLoader", "Size Z: " + std::to_string(sizez));

            // Read XYZI chunk
            input.read(chunkname, 4);
            if (strncmp(chunkname, "XYZI ", 4) != 0) {
                Log::message(Log::ERROR, "VoxLoader", "Expected XYZI chunk");
                input.close();
                throw;
            }

            input.read(reinterpret_cast<char *>(&chunkSize), sizeof(int));
            input.read(reinterpret_cast<char *>(&chunkChildren), sizeof(int));

            Log::message(Log::DEBUG, "VoxLoader", "--" + std::string(chunkname) + " Chunk --");
            Log::message(Log::DEBUG, "VoxLoader", "Chunk size: " + std::to_string(chunkSize));
            Log::message(Log::DEBUG, "VoxLoader", "Children size: " + std::to_string(chunkChildren));

            // Data
            int numvoxels;
            input.read(reinterpret_cast<char *>(&numvoxels), sizeof(int));

            Log::message(Log::DEBUG, "VoxLoader", "Number of voxels: " + std::to_string(numvoxels));

            std::vector<std::vector<std::vector<unsigned int>>> data;

            VoxelModel model(sizex, sizey, sizez);

            unsigned char x, y, z, c;

            for (int i = 0; i < numvoxels; i = i + 1) {
                input.read((char *) &x, 1);
                input.read((char *) &y, 1);
                input.read((char *) &z, 1);
                input.read((char *) &c, 1);
                model.setDataAtPoint(x, y, z, c);
            }

            models.push_back(model);
        } else if (strncmp(chunkname, "RGBA ", 4) == 0) {
            customColorPalette = true;

            colorPalette.push_back(0);

            for (int i = 0; i < 255; i = i + 1) {
                unsigned int c;
                input.read(reinterpret_cast<char *>(&c), sizeof(int));
                colorPalette.push_back(c);

            }
        } else {
            Log::message(Log::DEBUG, "VoxLoader", "Unknown chunk, ignoring.");
            input.ignore(chunkSize);
            input.ignore(chunkChildren);
        }
    }

    if(customColorPalette) {
        for(VoxelModel& model : models) {
            model.setColorPalette(colorPalette);
        }
    }

    for(VoxelModel& model : models) {
        model.mesh();
    }

    Log::message(Log::DEBUG, "VoxLoader", "Finished!");
    return models;
}