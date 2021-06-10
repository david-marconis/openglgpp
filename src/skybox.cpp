#include "skybox.hpp"

unsigned int skyboxIndices[] = {
    // front
    0, 1, 2,
    2, 1, 3,
    // right
    2, 3, 5,
    5, 3, 7,
    // back
    5, 7, 4,
    4, 7, 6,
    // left
    4, 6, 0,
    0, 6, 1,
    // top
    4, 0, 5,
    5, 0, 2,
    // bottom
    1, 6, 3,
    3, 6, 7};

float skyboxVertices[] = {
    -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

    -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

Skybox::Skybox()
{
}

Skybox::Skybox(std::vector<std::string> faceLocations)
{
    skyShader = new Shader();
    skyShader->fromFile("resources/skybox.vert", "resources/skybox.frag");
    uniformProjection = skyShader->getProjectionLocation();
    uniformView = skyShader->getViewLocation();

    int width;
    int height;
    int bitDepth;

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &textureId);

    for (size_t i = 0; i < 6; i++)
    {
        stbi_set_flip_vertically_on_load(false);
        std::string fileLocation = "resources/skybox/" + faceLocations[i];
        unsigned char *texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
        if (i == 0)
        {
            glTextureStorage2D(textureId, 1, GL_RGB8, width, height);
        }

        if (!texData)
        {
            std::cerr << "Failed to find: " << fileLocation << std::endl;
            return;
        }
        glTextureSubImage3D(textureId, 0, 0, 0, i, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, texData);
        stbi_image_free(texData);
    }

    glTextureParameteri(textureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(textureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    skyMesh = new Mesh();
    skyMesh->createMesh(skyboxVertices, skyboxIndices, 64, 36);
}

void Skybox::drawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
    viewMatrix = glm::mat4(glm::mat3(viewMatrix));

    glDepthMask(GL_FALSE);

    skyShader->useShader();
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glBindTextureUnit(0, textureId);

    skyMesh->renderMesh();

    glDepthMask(GL_TRUE);
}
