#pragma once

#include <vector>

#include "light.hpp"
#include "omniShadowMap.hpp"

class PointLight : public Light
{
protected:
    glm::vec3 position;
    GLfloat constant;
    GLfloat linear;
    GLfloat quadratic;
    GLfloat farPlane;

public:
    PointLight();
    PointLight(
        glm::vec3 color,
        glm::vec3 position,
        GLfloat ambientIntensity, GLfloat diffuseIntensity,
        GLfloat constant, GLfloat linear, GLfloat quadratic,
        GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far);
    ~PointLight(){};

    void useLight(
        GLuint colorLocation,
        GLuint positionLocation,
        GLuint ambientIntensityLocation,
        GLuint diffuseIntensityLocation,
        GLuint constantLocation,
        GLuint linearLocation,
        GLuint quadraticLocation);

    std::vector<glm::mat4> calculateLightTransform();

    GLfloat getFarPlane() { return farPlane; }
    glm::vec3 getPosition() { return position; }
};
