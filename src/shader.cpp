#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

Shader::Shader()
{
    shaderId = 0;
    pointLightCount = 0;
    spotLightCount = 0;
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

GLuint Shader::getArrayLocation(
    size_t i, std::string base,
    std::string modifier = "",
    std::string value = "")
{
    std::string combined = base + "[" + std::to_string(i) + "]" + modifier + value;
    return glGetUniformLocation(shaderId, combined.c_str());
}

void Shader::setPointLightUniforms(UniformPointLight *pointLight, size_t i, std::string base, std::string modifier)
{
    pointLight->light.uniformColor = getArrayLocation(i, base, modifier, ".base.color");
    pointLight->light.uniformAmbientIntensity = getArrayLocation(i, base, modifier, ".base.ambientIntensity");
    pointLight->light.uniformDiffuseIntensity = getArrayLocation(i, base, modifier, ".base.diffuseIntensity");
    pointLight->uniformPosition = getArrayLocation(i, base, modifier, ".position");
    pointLight->uniformConstant = getArrayLocation(i, base, modifier, ".constant");
    pointLight->uniformLinear = getArrayLocation(i, base, modifier, ".linear");
    pointLight->uniformQuadratic = getArrayLocation(i, base, modifier, ".quadratic");
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
    compileProgram();
}

void Shader::compileShader(const char *vertexCode, const char *fragmentCode, const char *geometryCode)
{
    shaderId = glCreateProgram();
    if (!shaderId)
    {
        std::cerr << "Error creating shader program!" << std::endl;
        return;
    }
    addShader(vertexCode, GL_VERTEX_SHADER);
    addShader(geometryCode, GL_GEOMETRY_SHADER);
    addShader(fragmentCode, GL_FRAGMENT_SHADER);
    compileProgram();
}

void Shader::compileProgram()
{
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
    uniformDirectionalLight.light.uniformColor = glGetUniformLocation(shaderId, "directionalLight.base.color");
    uniformDirectionalLight.light.uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.light.uniformDiffuseIntensity = glGetUniformLocation(shaderId, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderId, "directionalLight.direction");
    uniformSpecularIntensity = glGetUniformLocation(shaderId, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderId, "material.shininess");
    uniformPointLightCount = glGetUniformLocation(shaderId, "pointLightCount");
    uniformSpotLightCount = glGetUniformLocation(shaderId, "spotLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        setPointLightUniforms(&uniformPointLight[i], i, "pointLights", "");
    }
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        UniformSpotLight *spotLight = &uniformSpotLight[i];
        std::string base = "spotLights";
        setPointLightUniforms(&spotLight->pointLight, i, base, ".pointLight");
        spotLight->uniformDirection = getArrayLocation(i, base, "", ".direction");
        spotLight->uniformEdge = getArrayLocation(i, base, "", ".edge");
    }

    uniformDirectionalLightTransform = glGetUniformLocation(shaderId, "directionalLightTransform");
    uniformTexture = glGetUniformLocation(shaderId, "textureSampler");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderId, "directionalShadowMap");

    uniformOmniLightPosition = glGetUniformLocation(shaderId, "lightPosition");
    uniformFarPlane = glGetUniformLocation(shaderId, "farPlane");

    for (size_t i = 0; i < 6; i++)
    {
        uniformLightMatrices[i] = getArrayLocation(i, "lightMatrices");
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

void Shader::fromFile(const char *vertexFile, const char *fragmentFile, const char *geometryFile)
{
    std::string vertexString = readFile(vertexFile);
    std::string fragmentString = readFile(fragmentFile);
    std::string geometryString = readFile(geometryFile);

    const char *vertexCode = vertexString.c_str();
    const char *fragmentCode = fragmentString.c_str();
    const char *geometryCode = geometryString.c_str();

    compileShader(vertexCode, fragmentCode, geometryCode);
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
        uniformDirectionalLight.light.uniformColor,
        uniformDirectionalLight.uniformDirection,
        uniformDirectionalLight.light.uniformAmbientIntensity,
        uniformDirectionalLight.light.uniformDiffuseIntensity);
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
        UniformPointLight l = uniformPointLight[i];
        pointLights[i].useLight(
            l.light.uniformColor,
            l.uniformPosition,
            l.light.uniformAmbientIntensity,
            l.light.uniformDiffuseIntensity,
            l.uniformConstant,
            l.uniformLinear,
            l.uniformQuadratic);
    }
}

void Shader::setSpotLights(SpotLight *spotLights, unsigned int lightCount)
{
    if (lightCount > MAX_SPOT_LIGHTS)
    {
        spotLightCount = MAX_SPOT_LIGHTS;
    }
    glUniform1i(uniformSpotLightCount, lightCount);
    for (size_t i = 0; i < lightCount; i++)
    {
        UniformSpotLight s = uniformSpotLight[i];
        spotLights[i].useLight(
            s.pointLight.light.uniformColor,
            s.pointLight.uniformPosition,
            s.uniformDirection,
            s.pointLight.light.uniformAmbientIntensity,
            s.pointLight.light.uniformDiffuseIntensity,
            s.pointLight.uniformConstant,
            s.pointLight.uniformLinear,
            s.pointLight.uniformQuadratic,
            s.uniformEdge);
    }
}

void Shader::setTexture(GLuint textureUnit)
{
    glUniform1i(uniformTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint textureUnit)
{
    glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4 *lightTransform)
{
    glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lightTransform));
}

void Shader::setLightMatrices(std::vector<glm::mat4> lightMatrices)
{
    for (size_t i = 0; i < 6; i++)
    {
        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
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

GLuint Shader::getSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}

GLuint Shader::getShininessLocation()
{
    return uniformShininess;
}

GLuint Shader::getOmniLightPositionLocation()
{
    return uniformOmniLightPosition;
}

GLuint Shader::getFarPlaneLocation()
{
    return uniformFarPlane;
}
