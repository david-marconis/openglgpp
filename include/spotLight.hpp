#pragma once

#include "pointLight.hpp"

class SpotLight : public PointLight
{

    glm::vec3 direction;
    GLfloat edge;
    GLfloat processedEdge;

public:
    SpotLight();

    SpotLight(
        glm::vec3 color,
        glm::vec3 position,
        glm::vec3 direction,
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat constant, GLfloat linear, GLfloat quadratic,
        GLfloat edge,
        GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far);

    void useLight(
        GLuint colorLocation,
        GLuint positionLocation,
        GLuint directionLocation,
        GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation,
        GLuint constantLocation,
        GLuint linearLocation,
        GLuint quadraticLocation,
        GLuint edgeLocation);
    void setFlash(glm::vec3 position, glm::vec3 direction);
    ~SpotLight(){};
};
