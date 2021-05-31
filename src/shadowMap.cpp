#include "shadowMap.hpp"

bool ShadowMap::init(GLuint width, GLuint height)
{
    shadowHeight = height;
    shadowWidth = width;

    glCreateFramebuffers(1, &fbo);
    glCreateTextures(GL_TEXTURE_2D, 1, &shadowMap);
    glTextureStorage2D(shadowMap, 1, GL_DEPTH_COMPONENT32F, width, height);

    glTextureParameteri(shadowMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(shadowMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTextureParameterfv(shadowMap, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTextureParameteri(shadowMap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(shadowMap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glNamedFramebufferTexture(fbo, GL_DEPTH_ATTACHMENT, shadowMap, 0);
    glNamedFramebufferDrawBuffer(fbo, GL_NONE);
    glNamedFramebufferReadBuffer(fbo, GL_NONE);

    return checkFramebufferStatus();
}

void ShadowMap::write()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void ShadowMap::read(GLuint textureUnit)
{
    glBindTextureUnit(textureUnit, shadowMap);
}

ShadowMap::~ShadowMap()
{
    if (fbo)
    {
        glDeleteFramebuffers(1, &fbo);
    }
    if (shadowMap)
    {
        glDeleteTextures(1, &shadowMap);
    }
}

bool ShadowMap::checkFramebufferStatus()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Framebuffer error: " + status;
        return false;
    }
    return true;
}
