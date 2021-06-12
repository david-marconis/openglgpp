#include "transform.hpp"

Transform::Transform()
{
    model = glm::mat4(1.0f);
    translation = glm::vec3(0.0f);
    yaw = 0.0f;
    pitch = 0.0f;
    scaleVector = glm::vec3(1.0f);
    position = glm::vec3(0.0f);
}

void Transform::recalculateMatrix()
{
    model = glm::mat4(1.0f);
    model = glm::translate(model, translation);
    model = glm::rotate(model, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, position);
    model = glm::scale(model, scaleVector);
}
void Transform::setUniform(GLuint uniformModel)
{
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

void Transform::scale(glm::vec3 const &scale)
{
    scaleVector = scale;
    recalculateMatrix();
}

void Transform::turn(float yaw, float pitch)
{
    this->yaw = glm::radians(yaw);
    this->pitch = glm::radians(pitch);
    recalculateMatrix();
}
void Transform::move2(float right, float front, float up)
{
    translation = glm::vec3(right, up, front);
    recalculateMatrix();
}

void Transform::move(float right, float front, float up)
{
}

void Transform::reposition(glm::vec3 newPostion)
{
    position = newPostion;
    recalculateMatrix();
}
