#pragma once

#include "light.hpp"

class DirectionalLight : public Light
{
    glm::vec3 direction;

public:
    DirectionalLight();
    ~DirectionalLight(){};

    DirectionalLight(
        glm::vec3 color,
        glm::vec3 direction,
        GLfloat ambientIntensity,
        GLfloat diffuseIntensity,
        GLfloat shadowWidth,
        GLfloat shadowHeight);

    void useLight(
        GLuint colorLocation,
        GLuint directionLocation,
        GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation);
        
    glm::mat4 calculateLightTransform();
};
