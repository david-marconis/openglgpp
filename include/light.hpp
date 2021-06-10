#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "shadowMap.hpp"

class Light
{
protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
    ShadowMap *shadowMap;
    glm::mat4 lightProjection;
    bool isOn = true;

public:
    Light();
    ~Light(){};

    Light(
        glm::vec3 color,
        GLfloat ambientIntensity,
        GLfloat diffuseIntensity,
        GLfloat shadowWidth,
        GLfloat shadowHeight);

    void useLight(
        GLuint colorLocation,
        GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation);
    ShadowMap *getShadowMap() { return shadowMap; };
    void toggle() { isOn = !isOn; }
};
