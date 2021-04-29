#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "input.hpp"

class WindowManager
{
    GLFWwindow *window;
    int bufferHeight;
    int bufferWidth;
    GLint width;
    GLint height;

public:
    Input *input;

    WindowManager(GLint windowWidth, GLint windowHeight) : width(windowWidth), height(windowHeight){};
    ~WindowManager();
    int init();
    int getBufferWidth() { return bufferWidth; }
    int getBufferHeight() { return bufferHeight; }
    bool shouldClose() { return glfwWindowShouldClose(window); }
    void swapBuffers() { glfwSwapBuffers(window); }
};
