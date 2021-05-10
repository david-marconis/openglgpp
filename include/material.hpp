#pragma once

#include <GL/glew.h>

class Material
{
    GLfloat specularIntensity;
    GLfloat shininess;

public:
    Material();
    Material(GLfloat specularIntensity, GLfloat shininess)
        : specularIntensity(specularIntensity),
          shininess(shininess){};
    ~Material();

    void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
};
