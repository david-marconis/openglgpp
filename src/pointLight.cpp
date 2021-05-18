#include "pointLight.hpp"

PointLight::PointLight()
    : Light()
{
    this->position = glm::vec3(1.0f);
    this->constant = 1;
    this->linear = 0;
    this->quadratic = 0;
}

PointLight::PointLight(
    glm::vec3 color,
    glm::vec3 position,
    GLfloat ambientIntensity, GLfloat diffuseIntensity,
    GLfloat constant, GLfloat linear, GLfloat quadratic)
    : Light(color, ambientIntensity, diffuseIntensity)
{
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

void PointLight::useLight(
    GLuint colorLocation,
    GLuint positionLocation,
    GLuint ambientIntensityLocation,
    GLuint diffuseIntensityLocation,
    GLuint constantLocation,
    GLuint linearLocation,
    GLuint quadraticLocation)
{
    Light::useLight(colorLocation, ambientIntensityLocation, diffuseIntensityLocation);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(quadraticLocation, quadratic);
}
