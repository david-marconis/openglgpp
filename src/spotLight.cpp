#include "spotLight.hpp"

SpotLight::SpotLight() : PointLight()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0.0f;
}

SpotLight::SpotLight(
    glm::vec3 color,
    glm::vec3 position,
    glm::vec3 direction,
    GLfloat ambientIntensity, GLfloat diffuseIntensity,
    GLfloat constant, GLfloat linear, GLfloat quadratic,
    GLfloat edge,
    GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far)
    : PointLight(color, position, ambientIntensity, diffuseIntensity, constant, linear, quadratic,
                 shadowWidth, shadowHeight, near, far)
{
    this->direction = glm::normalize(direction);
    this->edge = edge;
    this->processedEdge = cosf(glm::radians(edge));
}

void SpotLight::useLight(
    GLuint colorLocation,
    GLuint positionLocation,
    GLuint directionLocation,
    GLuint ambientIntensityLocation,
    GLuint diffuseIntensityLocation,
    GLuint constantLocation,
    GLuint linearLocation,
    GLuint quadraticLocation,
    GLuint edgeLocation)
{
    PointLight::useLight(
        colorLocation,
        positionLocation,
        ambientIntensityLocation,
        diffuseIntensityLocation,
        constantLocation,
        linearLocation,
        quadraticLocation);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::setFlash(glm::vec3 position, glm::vec3 direction)
{
    this->direction = direction;
    this->position = position;
}
