#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

class Light
{
    glm::vec3 color;
    GLfloat ambientIntensity;

    glm::vec3 direction;
    GLfloat diffuseIntensity;

public:
    Light();
    ~Light(){};

    Light(
        glm::vec3 color,
        glm::vec3 direction,
        GLfloat ambientIntensity,
        GLfloat diffuseIntensity);

    void useLight(
        GLuint colorLocation,
        GLuint directionLocation,
        GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation);
};
