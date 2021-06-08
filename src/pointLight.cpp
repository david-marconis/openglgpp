#include <glm/gtc/matrix_transform.hpp>

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
    GLfloat constant, GLfloat linear, GLfloat quadratic,
    GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far)
    : Light(color, ambientIntensity, diffuseIntensity, shadowWidth, shadowHeight)
{
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
    this->farPlane = far;
    float aspect = (float)shadowWidth / (float)shadowHeight;
    lightProjection = glm::perspective(glm::radians(90.0f), aspect, near, far);

    shadowMap = new OmniShadowMap();
    shadowMap->init(shadowWidth, shadowHeight);
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

std::vector<glm::mat4> PointLight::calculateLightTransform()
{
    std::vector<glm::mat4> lightMatrices;
    lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.push_back(lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    return lightMatrices;
}
