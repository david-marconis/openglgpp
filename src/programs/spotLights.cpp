#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.hpp"
#include "shader.hpp"
#include "windowManager.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "directionalLight.hpp"
#include "pointLight.hpp"
#include "spotLight.hpp"
#include "material.hpp"

const float toRadians = M_PI / 180.0f;

static const char *vertexShader = "resources/shader.vert";
static const char *fragmentShader = "resources/shader.frag";

std::vector<Mesh *> meshList;
std::vector<Shader *> shaders;

void calculateAverageNormals(
    unsigned int *indices,
    unsigned int indexCount,
    GLfloat *vertices,
    unsigned int vertexCount,
    unsigned int vertexLength,
    unsigned int normalOffset)
{
    for (size_t i = 0; i < indexCount; i += 3)
    {
        unsigned int in[3];
        for (size_t j = 0; j < 3; j++)
        {
            in[j] = indices[i + j] * vertexLength;
        }

        glm::vec3 v1(0.0f);
        glm::vec3 v2(0.0f);
        for (size_t j = 0; j < 3; j++)
        {
            v1[j] = vertices[in[1] + j] - vertices[in[0] + j];
            v2[j] = vertices[in[2] + j] - vertices[in[0] + j];
        }

        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        for (size_t j = 0; j < 3; j++)
        {
            vertices[in[0] + normalOffset + j] += normal[j];
            vertices[in[1] + normalOffset + j] += normal[j];
            vertices[in[2] + normalOffset + j] += normal[j];
        }
    }

    for (size_t i = 0; i < vertexCount / vertexLength; i++)
    {
        unsigned int nOffset = i * vertexLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void initObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2};

    GLfloat vertices[] = {
        -1.0f, -1.0f, -0.6f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -0.6f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f};

    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3};
    GLfloat floorVertices[] = {-10.0f, 0.0f, -10.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 10.0f, 0.0f, -10.0f, 10.0f, 0.0f, 0.0f, -1.0f, 0.0f, -10.0f, 0.0f, 10.0f, 0.0f, 10.0f, 0.0f, -1.0f, 0.0f, 10.0f, 0.0f, 10.0f, 10.0f, 10.0f, 0.0f, -1.0f, 0.0f};

    unsigned int vertexCount = 32;
    unsigned int indexCount = 12;
    calculateAverageNormals(indices, indexCount, vertices, vertexCount, 8, 5);

    Mesh *mesh = new Mesh();
    mesh->createMesh(vertices, indices, vertexCount, indexCount);
    meshList.push_back(mesh);

    Mesh *mesh2 = new Mesh();
    mesh2->createMesh(vertices, indices, vertexCount, indexCount);
    meshList.push_back(mesh2);

    Mesh *mesh3 = new Mesh();
    mesh3->createMesh(floorVertices, floorIndices, vertexCount, 6);
    meshList.push_back(mesh3);
}

void initShaders()
{
    Shader *shader1 = new Shader();
    shader1->fromFile(vertexShader, fragmentShader);
    shaders.push_back(shader1);
}

int main()
{
    WindowManager windowManager(2732, 1536);
    windowManager.init();
    initObjects();
    initShaders();
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 3.0f, 0.1f);
    windowManager.input->registerMovable(&camera);

    Texture brick = Texture("resources/brick.png");
    brick.loadTexture();
    Texture dirt = Texture("resources/dirt.png");
    dirt.loadTexture();
    Texture plain = Texture("resources/plain.png");
    plain.loadTexture();

    Material shinyMaterial = Material(4.0f, 256.0f);
    Material dullMaterial = Material(0.3f, 4);

    DirectionalLight directionalLight = DirectionalLight(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(2.0f, -1.0f, -2.0f),
        0.1f,
        0.2f);
    PointLight pointLights[MAX_POINT_LIGHTS];
    SpotLight spotLights[MAX_SPOT_LIGHTS];

    unsigned int pointLightCount = 0;

    pointLights[0] = PointLight(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(4.0, 0.0f, -7.0f),
        0.1f, 0.1f,
        0.3f, 0.2f, 0.1f);
    pointLightCount++;

    pointLights[1] = PointLight(
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(-4.0, 2.0f, -7.0f),
        0.1f, 0.1f,
        0.3f, 0.1f, 0.1f);
    pointLightCount++;

    unsigned int spotLightCount = 0;
    spotLights[0] = SpotLight(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0, -2.0f, -7.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        0.1f, 1.0f,
        1.0f, 0.0f, 0.0f,
        10.0f);
    spotLightCount++;

    spotLights[1] = SpotLight(
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.0, -4.9f, -7.0f),
        glm::vec3(-100.0f, -1.0f, 0.0f),
        0.1f, 1.0f,
        1.0f, 0.0f, 0.0f,
        20.0f);
    spotLightCount++;

    GLfloat aspectRatio = (GLfloat)windowManager.getBufferWidth() /
                          (GLfloat)windowManager.getBufferHeight();
    glm::mat4 projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformEyePosition = 0;
    GLuint uniformSpecularIntensity = 0;
    GLuint uniformShininess = 0;

    GLfloat deltaTime = 0.0f;
    GLfloat lastTime = 0.0f;

    while (!windowManager.shouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        windowManager.input->update(deltaTime);

        shaders[0]->useShader();
        uniformProjection = shaders[0]->getProjectionLocation();
        uniformModel = shaders[0]->getModelLocation();
        uniformView = shaders[0]->getViewLocation();
        uniformEyePosition = shaders[0]->getEyePositionLocation();
        uniformSpecularIntensity = shaders[0]->getSpecularIntensityLocation();
        uniformShininess = shaders[0]->getShininessLocation();

        glm::vec3 lightPosition = camera.getCameraPosition();
        lightPosition.y -= 0.3f;

        spotLights[0].setFlash(lightPosition, camera.getCameraDirection());
        shaders[0]->setDirectionalLight(&directionalLight);
        shaders[0]->setPointLights(pointLights, pointLightCount);
        shaders[0]->setSpotLights(spotLights, spotLightCount);
        glm::vec3 cameraPosition = camera.getCameraPosition();
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, -10.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        brick.useTexture();
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, -10.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        dirt.useTexture();
        dullMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, -10.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        plain.useTexture();
        shinyMaterial.useMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[2]->renderMesh();

        glUseProgram(0);

        windowManager.swapBuffers();
    }
    return 0;
}
