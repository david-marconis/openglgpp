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
    void useShader();
    void clearShader();
};
