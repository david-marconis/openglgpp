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
    glm::vec3 noColor = glm::vec3(0.0f);
    glm::vec3 *toggledColor = &color;

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
    void toggle() { toggledColor = toggledColor == &noColor ? &color : &noColor; }
};
