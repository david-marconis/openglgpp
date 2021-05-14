#include "directionalLight.hpp"

DirectionalLight::DirectionalLight() : Light()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(
    glm::vec3 color,
    glm::vec3 direction,
    GLfloat ambientIntensity,
    GLfloat diffuseIntensity)
    : Light(color, ambientIntensity, diffuseIntensity)
{
    this->direction = direction;
}

void DirectionalLight::useLight(
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
