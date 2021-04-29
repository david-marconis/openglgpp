#include "input.hpp"

#include <algorithm>

#include <GLFW/glfw3.h>

void Input::registerMovable(Movable *movable)
{
    auto index = std::find(movables.begin(), movables.end(), movable);
    if (index == movables.end())
    {
        movables.push_back(movable);
    }
}

void Input::unregisterMovable(Movable *movable)
{
    auto index = std::find(movables.begin(), movables.end(), movable);
    if (index != movables.end())
    {
        movables.erase(index);
    }
}

void Input::update(float deltaTime)
{
    float right = (keyValues[rightKey] - keyValues[leftKey]) * deltaTime;
    float front = (keyValues[upKey] - keyValues[downKey]) * deltaTime;

    if (right != 0 || front != 0)
    {
        for (Movable *movable : movables)
        {
            movable->move(right, front);
        }
    }
}

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    Input *input = static_cast<Input *>(glfwGetWindowUserPointer(window));

    switch (action)
    {
    case GLFW_PRESS:
        input->keyValues[key] = 1.0f;
        break;
    case GLFW_RELEASE:
        input->keyValues[key] = 0.0f;
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void Input::mouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    Input *input = static_cast<Input *>(glfwGetWindowUserPointer(window));

    if (!input->mouseHasMoved)
    {
        input->lastX = xPos;
        input->lastY = yPos;
        input->mouseHasMoved = true;
    }
    input->xChange = xPos - input->lastX;
    input->yChange = input->lastY - yPos;
    input->lastX = xPos;
    input->lastY = yPos;
    for (Movable *movable : input->movables)
    {
        movable->turn(input->xChange, input->yChange);
    }
}
