#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
    GLuint shaderId;
    GLuint uniformProjection;
    GLuint uniformModel;
    GLuint uniformView;
    GLuint uniformAmbientIntensity;
    GLuint uniformAmbientColor;

    void compileShader(const char *vertexCode, const char *fragmentCode);
    void addShader(const char *shaderSource, GLenum shaderType);

public:
    Shader();
    ~Shader();
    void fromString(const char *vertexCode, const char *fragmentCode);
    void fromFile(const char *vertexFile, const char* fragmentFile);
    std::string readFile(const char *filename);
    GLuint getProjectionLocation();
    GLuint getModelLocation();
    GLuint getViewLocation();
    GLuint getAmbientIntensityLocation();
    GLuint getAmbientColorLocation();
    void useShader();
    void clearShader();
};
