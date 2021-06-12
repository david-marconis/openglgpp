#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "movable.hpp"

class Transform : public Movable
{
    glm::mat4 model;
    glm::vec3 translation;
    glm::vec3 scaleVector;
    glm::vec3 position;
    float yaw;
    float pitch;

    void recalculateMatrix();

public:
    Transform();
    ~Transform(){};
    void reposition(glm::vec3 newPosition);
    void setUniform(GLuint uniformModel);
    void scale(glm::vec3 const &v);

    void turn(float yaw, float pitch);
    void move(float right, float front, float up);
    void move2(float right, float front, float up);
};
