#include "shader.hpp"

Shader::Shader()
{
    pointLightCount = 0;
    shaderId = 0;
    uniformModel = 0;
    uniformProjection = 0;
    uniformView = 0;
}

Shader::~Shader()
{
    clearShader();
}

GLuint Shader::getUniformArrayLocation(char *locationBuffer, const char *variable, size_t i)
{
    snprintf(locationBuffer, sizeof(locationBuffer), variable, i);
    return glGetUniformLocation(shaderId, locationBuffer);
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
    uniformView = glGetUniformLocation(shaderId, "view");
    uniformEyePosition = glGetUniformLocation(shaderId, "eyePosition");
    uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderId, "directionalLight.base.color");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
    uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderId, "material.shininess");
    uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderId, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%ld].quadratic", i);
		uniformPointLight[i].uniformQuadratic = glGetUniformLocation(shaderId, locBuff);
    }
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
    uniformView = 0;
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

void Shader::setDirectionalLight(DirectionalLight *light)
{
    light->useLight(
        uniformDirectionalLight.uniformColor,
        uniformDirectionalLight.uniformDirection,
        uniformDirectionalLight.uniformAmbientIntensity,
        uniformDirectionalLight.uniformDiffuseIntensity);
}

void Shader::setPointLights(PointLight *pointLights, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS)
    {
        pointLightCount = MAX_POINT_LIGHTS;
    }
    glUniform1i(uniformPointLightCount, lightCount);
    for (size_t i = 0; i < lightCount; i++)
    {
        auto l = uniformPointLight[i];
        pointLights[i].useLight(
            l.uniformColor,
            l.uniformPosition,
            l.uniformAmbientIntensity,
            l.uniformDiffuseIntensity,
            l.uniformConstant,
            l.uniformLinear,
            l.uniformQuadratic);
    }
}

GLuint Shader::getModelLocation()
{
    return uniformModel;
}

GLuint Shader::getProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::getViewLocation()
{
    return uniformView;
}

GLuint Shader::getEyePositionLocation()
{
    return uniformEyePosition;
}

GLuint Shader::getAmbientIntensityLocation()
{
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::getAmbientColorLocation()
{
    return uniformDirectionalLight.uniformColor;
}

GLuint Shader::getDiffuseIntensityLocation()
{
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::getDirectionLocation()
{
    return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::getSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation()
{
    return uniformShininess;
}
