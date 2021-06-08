#pragma once

#include <iostream>

#include <GL/glew.h>

class ShadowMap
{
protected:
    GLuint fbo;
    GLuint shadowMap;
    GLuint shadowWidth;
    GLuint shadowHeight;

public:
    ShadowMap(){};
    ~ShadowMap();

    virtual bool init(GLuint width, GLuint height);
    void write();
    void read(GLenum textureUnit);
    GLuint getShadowWidth() { return shadowWidth; };
    GLuint getShadowHeight() { return shadowHeight; };
    bool checkFramebufferStatus();
};
