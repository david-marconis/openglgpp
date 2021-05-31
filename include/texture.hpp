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

    bool loadTexture(GLenum internalFormat, GLenum format);

public:
    Texture() {}
    Texture(const char *fileLocation) : fileLocation(fileLocation){};
    ~Texture();

    bool loadTexture();
    bool loadTextureWithAlpha();
    void useTexture();
    void clearTexture();
};
