#pragma once

#include "light.hpp"

class PointLight : public Light
{
protected:
    glm::vec3 position;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;

public:
    PointLight();
    PointLight(
        glm::vec3 color,
        glm::vec3 position,
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat constant, GLfloat linear, GLfloat quadratic);
    ~PointLight(){};

    void useLight(
        GLuint colorLocation,
        GLuint positionLocation,
        GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation,
        GLuint constantLocation,
        GLuint linearLocation,
        GLuint quadraticLocation);
};
