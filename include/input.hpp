#pragma once

#include <GLFW/glfw3.h>

#include "movable.hpp"

class Input
{
    bool mouseHasMoved = false;
    double lastX = 0.0f;
    double lastY = 0.0f;
    double xChange = 0.0f;
    double yChange = 0.0f;

    static const int upKey = GLFW_KEY_W;
    static const int leftKey = GLFW_KEY_A;
    static const int downKey = GLFW_KEY_S;
    static const int rightKey = GLFW_KEY_D;

    float keyValues[1024] = {0};

    std::vector<Movable *> movables;

public:
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
    static void mouseCallback(GLFWwindow *window, double xPos, double yPos);
    void registerMovable(Movable *movable);
    void unregisterMovable(Movable *movable);
    void update(float deltaTime);
};