#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

const GLint WIDTH = 800;
const GLint HEIGHT = 600;
const float toRadians = M_PI / 180.0f;

GLuint vao;
GLuint vbo;
GLuint ibo;
GLuint shaderProgram;
GLuint uniformModel;
GLuint uniformProjection;

float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.05f;

float currentAngle = 0.0f;

float currentScale = 0.1f;
float scaleIncrement = 0.01f;

const char *vertexShader = "                                          \n\
#version 450 core                                                     \n\
layout(location=0) in vec3 pos;                                       \n\
out vec4 vertexColor;                                                 \n\
uniform mat4 model;                                                   \n\
uniform mat4 projection;                                              \n\
void main(void) {                                                     \n\
    gl_Position = projection * model * vec4(pos.x, pos.y, pos.z, 1.0);             \n\
    vertexColor = vec4(clamp(pos, 0.0, 1.0), 1.0);                    \n\
}                                                                     \n\
";
const char *fragmentShader = "          \n\
#version 450 core                       \n\
in vec4 vertexColor;                    \n\
out vec4 color;                         \n\
void main()                             \n\
{                                       \n\
    color = vertexColor;                \n\
}";

void createTriangle()
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

    glCreateVertexArrays(1, &vao);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GL_FLOAT) * 3);

    glCreateBuffers(1, &ibo);
    glNamedBufferData(ibo, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vao, ibo);
}

void addShader(GLuint shaderProgram, const char *shaderSource, GLenum shaderType)
{
    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    glCompileShader(shaderId);
    GLint compileSuccess;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess)
    {
        GLchar errorLog[1024];
        glGetShaderInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        cerr << "Error compiling the " << shaderType << " shader: " << errorLog << endl;
        return;
    }
    glAttachShader(shaderProgram, shaderId);
    glDeleteShader(shaderId);
}

void compileShaders()
{
    shaderProgram = glCreateProgram();
    if (!shaderProgram)
    {
        cerr << "Error creating shader program!" << endl;
        return;
    }
    addShader(shaderProgram, vertexShader, GL_VERTEX_SHADER);
    addShader(shaderProgram, fragmentShader, GL_FRAGMENT_SHADER);

    GLint linkSuccess = 0;
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess)
    {
        GLchar errorLog[1024];
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        cerr << "Error linking program: " << errorLog << endl;
        return;
    }
    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &linkSuccess);
    if (!linkSuccess)
    {
        GLchar errorLog[1024];
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        cerr << "Error validating program: " << errorLog << endl;
        return;
    }

    uniformModel = glGetUniformLocation(shaderProgram, "model");
    uniformProjection = glGetUniformLocation(shaderProgram, "projection");
}

void updateDirection()
{
    triangleOffset += triangleIncrement;

    if (triangleOffset >= triangleMaxOffset || triangleOffset <= -triangleMaxOffset)
    {
        triangleIncrement = -triangleIncrement;
    }
    currentAngle += 1.0;
    if (currentAngle >= 360)
    {
        currentAngle -= 360;
    }
    currentScale += scaleIncrement;
    if (currentScale >= 1.0f || currentScale <= 0.1f)
    {
        scaleIncrement = -scaleIncrement;
    }
}

int main()
{
    cout << "Starting glfw" << endl;
    if (!glfwInit())
    {
        cerr << "GLFW init failed!" << endl;
        const char *description;
        int code = glfwGetError(&description);
        if (description)
        {
            cerr << "GLFW error: " << description << endl;
        }
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Awesome sauce", NULL, NULL);
    if (!window)
    {
        cerr << "Window creation failed!" << endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        cerr << "GLEW init failed: " << glewGetErrorString(glewStatus) << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }

    int bufferWidth;
    int bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    createTriangle();
    compileShaders();

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)(bufferWidth / bufferHeight), 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        updateDirection();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4, 0.4, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
