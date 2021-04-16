#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using std::cerr;
using std::cout;
using std::endl;

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

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
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
