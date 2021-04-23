#include "shader.hpp"

Shader::Shader()
{
    shaderId = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

Shader::~Shader()
{
    clearShader();
}

void Shader::compileShader(const char *vertexCode, const char *fragmentCode)
{
    shaderId = glCreateProgram();
    if (!shaderId)
    {
        std::cerr << "Error creating shader program!" << std::endl;
        return;
    }
    addShader(vertexCode, GL_VERTEX_SHADER);
    addShader(fragmentCode, GL_FRAGMENT_SHADER);

    GLint linkSuccess = 0;
    glLinkProgram(shaderId);
    glGetProgramiv(shaderId, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess)
    {
        GLchar errorLog[1024];
        glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error linking program: " << errorLog << std::endl;
        return;
    }
    glValidateProgram(shaderId);
    glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &linkSuccess);
    if (!linkSuccess)
    {
        GLchar errorLog[1024];
        glGetProgramInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error validating program: " << errorLog << std::endl;
        return;
    }

    uniformModel = glGetUniformLocation(shaderId, "model");
    uniformProjection = glGetUniformLocation(shaderId, "projection");
}

void Shader::fromString(const char *vertexCode, const char *fragmentCode)
{
    compileShader(vertexCode, fragmentCode);
}

void Shader::useShader()
{
    glUseProgram(shaderId);
}

void Shader::clearShader()
{
    if (shaderId != 0)
    {
        glDeleteProgram(shaderId);
        shaderId = 0;
    }
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::addShader(const char *shaderSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);
    GLint compileSuccess;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess)
    {
        GLchar errorLog[1024];
        glGetShaderInfoLog(shaderId, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error compiling the " << shaderType << " shader: " << errorLog << std::endl;
        return;
    }
    glAttachShader(shaderId, shader);
    glDeleteShader(shader);
}

void Shader::fromFile(const char *vertexFile, const char *fragmentFile)
{
    std::string vertexString = readFile(vertexFile);
    std::string fragmentString = readFile(fragmentFile);

    const char *vertexCode = vertexString.c_str();
    const char *fragmentCode = fragmentString.c_str();

    compileShader(vertexCode, fragmentCode);
}
std::string Shader::readFile(const char *filename)
{
    std::string contents;
    std::ifstream fileStream(filename, std::ios::in);

    if (!fileStream.is_open())
    {
        std::cout << "Failed to read " << filename << "! File doesn't exist." << std::endl;
        return "";
    }
    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        contents.append(line + "\n");
    }
    fileStream.close();
    return contents;
}

GLuint Shader::getModelLocation()
{
    return uniformModel;
}

GLuint Shader::getProjectionLocation()
{
    return uniformProjection;
}
