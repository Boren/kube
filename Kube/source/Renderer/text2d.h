#ifndef VOXLER_TEXT2D_H
#define VOXLER_TEXT2D_H

void initText2D(const char * texturePath);
void Text2DupdateResolution(int width, int height);
void printText2D(std::string text, int x, int y, int size);
void cleanupText2D();

#endif //VOXLER_TEXT2D_H
