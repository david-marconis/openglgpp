#include <iostream>
#include <algorithm>

#include "camera.hpp"

Camera::Camera(
    glm::vec3 position,
    glm::vec3 worldUp,
    GLfloat yaw,
    GLfloat pitch,
    GLfloat movementSpeed,
    GLfloat turnSpeed)
{
    this->position = position;
    this->worldUp = worldUp;
    this->yaw = yaw;
    this->pitch = pitch;
    this->movementSpeed = movementSpeed;
    this->turnSpeed = turnSpeed;

    this->front = glm::vec3(0.0f, 0.0f, -1.0f);
    update();
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(cross(right, front));
}

void Camera::turn(GLfloat yaw, GLfloat pitch)
{
    this->yaw += yaw * turnSpeed;
    this->pitch += pitch * turnSpeed;
    this->pitch = std::max(-89.0f, std::min(this->pitch, 89.0f));
    update();
}
void Camera::move(GLfloat right, GLfloat front)
{
    position += this->front * (front * movementSpeed) +
                this->right * (right * movementSpeed);
}

glm::mat4 Camera::calculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

Camera::~Camera()
{
}
