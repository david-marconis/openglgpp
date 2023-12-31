#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "directionalLight.hpp"
#include "pointLight.hpp"
#include "spotLight.hpp"
#include "constants.hpp"

class Shader
{
    int pointLightCount;
    int spotLightCount;

    struct UniformLight
    {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
    };

    struct
    {
        UniformLight light;
        GLuint uniformDirection;
    } uniformDirectionalLight;

    struct UniformPointLight
    {
        UniformLight light;
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformQuadratic;
    } uniformPointLight[MAX_POINT_LIGHTS];

    struct UniformSpotLight
    {
        UniformPointLight pointLight;
        GLuint uniformEdge;
        GLuint uniformDirection;
    } uniformSpotLight[MAX_SPOT_LIGHTS];

    struct
    {
        GLuint shadowMap;
        GLuint farPlane;
    } uniformOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

    GLuint shaderId;
    GLuint uniformPointLightCount;
    GLuint uniformSpotLightCount;
    GLuint uniformProjection;
    GLuint uniformModel;
    GLuint uniformView;
    GLuint uniformEyePosition;
    GLuint uniformSpecularIntensity;
    GLuint uniformShininess;
    GLuint uniformTexture;
    GLuint uniformDirectionalLightTransform;
    GLuint uniformDirectionalShadowMap;
    GLuint uniformOmniLightPosition;
    GLuint uniformFarPlane;

    GLuint uniformLightMatrices[6];

    GLuint getUniformArrayLocation(char *locationBuffer, const char *variable, size_t i);
    void compileShader(const char *vertexCode, const char *fragmentCode);
    void compileShader(const char *vertexCode, const char *fragmentCode, const char *geometryCode);
    void addShader(const char *shaderSource, GLenum shaderType);
    void compileProgram();

    GLuint getArrayLocation(size_t i, std::string base, std::string modifier, std::string value);
    void setPointLightUniforms(UniformPointLight *pointLight, size_t i, std::string base, std::string modifier);

public:
    Shader();
    ~Shader();
    void fromString(const char *vertexCode, const char *fragmentCode);
    void fromFile(const char *vertexFile, const char *fragmentFile);
    void fromFile(const char *vertexFile, const char *fragmentFile, const char *geometryFile);
    void validate();
    std::string readFile(const char *filename);
    GLuint getProjectionLocation();
    GLuint getModelLocation();
    GLuint getViewLocation();
    GLuint getEyePositionLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColorLocation();
    GLuint getDiffuseIntensityLocation();
    GLuint getDirectionLocation();
    GLuint getSpecularIntensityLocation();
    GLuint getShininessLocation();
    GLuint getOmniLightPositionLocation();
    GLuint getFarPlaneLocation();
    void useShader();
    void clearShader();
    void setDirectionalLight(DirectionalLight *light);
    void setPointLights(PointLight *pointLights, unsigned int lightCount, GLuint textureUnit, unsigned int offset);
    void setSpotLights(SpotLight *spotLights, unsigned int lightCount, GLuint textureUnit, unsigned int offset);
    void setTexture(GLuint textureUnit);
    void setDirectionalShadowMap(GLuint textureUnit);
    void setDirectionalLightTransform(glm::mat4 *lightTransform);
    void setLightMatrices(std::vector<glm::mat4> lightMatrices);
};
