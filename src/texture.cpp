#include <iostream>

#include "texture.hpp"

bool Texture::loadTexture(GLenum internalFormat, GLenum format)
{

    stbi_set_flip_vertically_on_load(true);
    unsigned char *texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if (!texData)
    {
        std::cerr << "Failed to find: " << fileLocation << std::endl;
        return false;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &textureId);

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(textureId, 1, internalFormat, width, height);
    glTextureSubImage2D(textureId, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, texData);
    glGenerateTextureMipmap(textureId);

    stbi_image_free(texData);
    return true;
}

bool Texture::loadTextureWithAlpha()
{
    return loadTexture(GL_RGBA8, GL_RGBA);
}

bool Texture::loadTexture()
{
    return loadTexture(GL_RGB8, GL_RGB);
}

void Texture::useTexture()
{
    glBindTextureUnit(0, textureId);
}

void Texture::clearTexture()
{
    glDeleteTextures(1, &textureId);
    textureId = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";
}

Texture::~Texture()
{
    clearTexture();
}
