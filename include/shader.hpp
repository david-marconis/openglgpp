#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "directionalLight.hpp"
#include "pointLight.hpp"
#include "constants.hpp"

class Shader
{
    int pointLightCount;

    struct
    {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        GLuint uniformDirection;

    } uniformDirectionalLight;

    struct
    {
        GLuint uniformColor;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;

        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformQuadratic;
    } uniformPointLight[MAX_POINT_LIGHTS];

    GLuint shaderId;
    GLuint uniformPointLightCount;
    GLuint uniformProjection;
    GLuint uniformModel;
    GLuint uniformView;
    GLuint uniformEyePosition;
    GLuint uniformSpecularIntensity;
    GLuint uniformShininess;

    GLuint getUniformArrayLocation(char *locationBuffer, const char *variable, size_t i);
    void compileShader(const char *vertexCode, const char *fragmentCode);
    void addShader(const char *shaderSource, GLenum shaderType);

public:
    Shader();
    ~Shader();
    void fromString(const char *vertexCode, const char *fragmentCode);
    void fromFile(const char *vertexFile, const char *fragmentFile);
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
    void useShader();
    void clearShader();
    void setDirectionalLight(DirectionalLight *light);
    void setPointLights(PointLight *pointLights, unsigned int lightCount);
};
