#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

class Light
{
    glm::vec3 color;
    GLfloat ambientIntensity;

public:
    Light();
    ~Light(){};

    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity);
    void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation);
};
