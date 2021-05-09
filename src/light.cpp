#include "light.hpp"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    ambientIntensity = 1;
    diffuseIntensity = 0.0f;
}

Light::Light(
    glm::vec3 color,
    glm::vec3 direction,
    GLfloat ambientIntensity,
    GLfloat diffuseIntensity)
{
    this->color = color;
    this->direction = direction;
    this->ambientIntensity = ambientIntensity;
    this->diffuseIntensity = diffuseIntensity;
}

void Light::useLight(
    GLuint colorLocation,
    GLuint directionLocation,
    GLuint ambientIntensityLocation,
    GLuint diffuseIntensityLocation)
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
