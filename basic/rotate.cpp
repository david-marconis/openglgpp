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
GLuint shaderProgram;
GLuint uniformModel;

float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.05f;

float currentAngle = 0.0f;

const char *vertexShader = "                                          \n\
#version 450 core                                                     \n\
layout(location=0) in vec3 pos;                                       \n\
uniform mat4 model;                                                  \n\
void main(void) {                                                     \n\
    gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0); \n\
}                                                                     \n\
";
const char *fragmentShader = "          \n\
#version 450 core                       \n\
out vec4 color;                         \n\
void main()                             \n\
{                                       \n\
    color = vec4(1.0, 0.0, 1.0, 1.0);   \n\
}";

void createTriangle()
{
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GL_FLOAT) * 3);
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

    createTriangle();
    compileShaders();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        updateDirection();
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(triangleOffset, 0.0f, 0.0f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
