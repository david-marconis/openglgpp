#pragma once

#include <GL/glew.h>

#include <stb_image.h>

class Texture
{
    GLuint textureId = 0;
    int width = 0;
    int height = 0;
    int bitDepth = 0;
    const char *fileLocation;

public:
    Texture(const char *fileLocation) : fileLocation(fileLocation){};
    ~Texture();

    void loadTexture();
    void useTexture();
    void clearTexture();
};
