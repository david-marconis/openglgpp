#include "light.hpp"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    toggledColor = color;
    ambientIntensity = 1;
    diffuseIntensity = 0.0f;
}

Light::Light(
    glm::vec3 color,
    GLfloat ambientIntensity,
    GLfloat diffuseIntensity,
    GLfloat shadowWidth,
    GLfloat shadowHeight)
{
    this->color = color;
    toggledColor = color;
    this->ambientIntensity = ambientIntensity;
    this->diffuseIntensity = diffuseIntensity;
    this->shadowMap = new ShadowMap();
    shadowMap->init(shadowWidth, shadowHeight);
}

void Light::useLight(
    GLuint colorLocation,
    GLuint ambientIntensityLocation,
    GLuint diffuseIntensityLocation)
{
    glUniform3f(colorLocation, toggledColor.x, toggledColor.y, toggledColor.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
