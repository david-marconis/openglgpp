#pragma once

#include "shadowMap.hpp"

class OmniShadowMap : public ShadowMap
{

public:
    OmniShadowMap();
    ~OmniShadowMap();

    bool init(GLuint width, GLuint height);
    void write();
    void read(GLenum textureUnit);
};
