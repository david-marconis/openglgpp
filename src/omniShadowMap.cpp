#include "omniShadowMap.hpp"

OmniShadowMap::OmniShadowMap() : ShadowMap() {}

bool OmniShadowMap::init(GLuint width, GLuint height)
{
    shadowWidth = width;
    shadowHeight = height;

    glCreateFramebuffers(1, &fbo);
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &shadowMap);
    glTextureStorage2D(shadowMap, 1, GL_DEPTH_COMPONENT32F, width, height);

    glTextureParameteri(shadowMap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(shadowMap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(shadowMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(shadowMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(shadowMap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, shadowMap, 0);
    glNamedFramebufferDrawBuffer(fbo, GL_NONE);
    glNamedFramebufferReadBuffer(fbo, GL_NONE);

    return checkFramebufferStatus();
}
