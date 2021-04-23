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

const float toRadians = M_PI / 180.0f;

static const char *vertexShader = "resources/shader.vert";
static const char *fragmentShader = "resources/shader.frag";

std::vector<Mesh *> meshList;
std::vector<Shader *> shaders;

void initObjects()
{
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2};

    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    Mesh *mesh = new Mesh();
    mesh->createMesh(vertices, indices, 12, 12);
    meshList.push_back(mesh);

    Mesh *mesh2 = new Mesh();
    mesh2->createMesh(vertices, indices, 12, 12);
    meshList.push_back(mesh2);
}

void initShaders()
{
    Shader *shader1 = new Shader();
    shader1->fromFile(vertexShader, fragmentShader);
    shaders.push_back(shader1);
}

int main()
{
    WindowManager windowManager(1800, 900);
    windowManager.init();
    initObjects();
    initShaders();

    GLfloat aspectRatio = (GLfloat)windowManager.getBufferWidth() /
                          (GLfloat)windowManager.getBufferHeight();
    glm::mat4 projection = glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f);
    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;

    while (!windowManager.shouldClose())
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaders[0]->useShader();
        uniformProjection = shaders[0]->getProjectionLocation();
        uniformModel = shaders[0]->getModelLocation();
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[0]->renderMesh();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.5f));

        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        meshList[1]->renderMesh();

        glUseProgram(0);

        windowManager.swapBuffers();
    }
    return 0;
}
