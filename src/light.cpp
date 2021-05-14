#include "light.hpp"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1;
    diffuseIntensity = 0.0f;
}

Light::Light(
    glm::vec3 color,
    GLfloat ambientIntensity,
    GLfloat diffuseIntensity)
{
    this->color = color;
    this->ambientIntensity = ambientIntensity;
    this->diffuseIntensity = diffuseIntensity;
}
