#include <algorithm>

#include "input.hpp"

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
    float front = (keyValues[forwardsKey] - keyValues[backwardsKey]) * deltaTime;
    float up = (keyValues[upKey] - keyValues[downKey]) * deltaTime;

    if (right != 0 || front != 0 || up != 0)
    {
        for (Movable *movable : movables)
        {
            movable->move(right, front, up);
        }
    }
    if (xChange != 0 || yChange != 0)
    {
        for (Movable *movable : movables)
        {
            movable->turn(xChange, yChange);
        }
        xChange = 0.0f;
        yChange = 0.0f;
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
        case lightKey:
            if (input->light)
            {
                input->light->toggle();
            }
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
}
