#include <iostream>

#include "windowManager.hpp"

WindowManager::~WindowManager()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

int WindowManager::init()
{
    std::cout << "Starting glfw" << std::endl;
    if (!glfwInit())
    {
        std::cerr << "GLFW init failed!" << std::endl;
        const char *description;
        int code = glfwGetError(&description);
        if (description)
        {
            std::cerr << "GLFW error: " << description << std::endl;
        }
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, "Awesome sauce", NULL, NULL);
    if (!window)
    {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        std::cerr << "GLEW init failed: " << glewGetErrorString(glewStatus) << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }

    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    return 0;
}
