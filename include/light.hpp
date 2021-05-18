#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

class Light
{
protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;

public:
    Light();
    ~Light(){};

    Light(
        glm::vec3 color,
        GLfloat ambientIntensity,
        GLfloat diffuseIntensity);
        
    void useLight(
        GLuint colorLocation,
        GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation);
};
