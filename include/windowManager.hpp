#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class WindowManager
{
    GLFWwindow *window;
    int bufferHeight;
    int bufferWidth;
    GLint width;
    GLint height;

public:
    WindowManager(GLint windowWidth, GLint windowHeight) : width(windowWidth), height(windowHeight){};
    ~WindowManager();
    int init();
    int getBufferWidth() { return bufferHeight; }
    int getBufferHeight() { return bufferWidth; }
    bool shouldClose() { return glfwWindowShouldClose(window); }
    void swapBuffers() { glfwSwapBuffers(window); }
};
