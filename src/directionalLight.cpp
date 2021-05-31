
#include <glm/gtc/matrix_transform.hpp>

#include "directionalLight.hpp"

DirectionalLight::DirectionalLight() : Light()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(
    glm::vec3 color,
    glm::vec3 direction,
    GLfloat ambientIntensity,
    GLfloat diffuseIntensity,
    GLfloat shadowWidth,
    GLfloat shadowHeight)
    : Light(color, ambientIntensity, diffuseIntensity, shadowWidth, shadowHeight)
{
    this->direction = direction;
    this->lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::useLight(
    GLuint colorLocation,
    GLuint directionLocation,
    GLuint ambientIntensityLocation,
    GLuint diffuseIntensityLocation)
{
    Light::useLight(colorLocation, ambientIntensityLocation, diffuseIntensityLocation);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

glm::mat4 DirectionalLight::calculateLightTransform()
{
    return lightProjection * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
