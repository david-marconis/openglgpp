#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "movable.hpp"

class Camera : public Movable
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat turnSpeed;

    void update();

public:
    Camera(
        glm::vec3 position,
        glm::vec3 worldUp,
        GLfloat yaw,
        GLfloat pitch,
        GLfloat movementSpeed,
        GLfloat turnSpeed);
    ~Camera();

    void turn(GLfloat yaw, GLfloat pitch);
    void move(GLfloat right, GLfloat front);

    glm::vec3 getCameraPosition() { return position; }
    glm::vec3 getCameraDirection() { return glm::normalize(front); }
    glm::mat4 calculateViewMatrix();
};
