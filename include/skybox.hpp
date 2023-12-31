#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "constants.hpp"
#include "shader.hpp"
#include "mesh.hpp"

class Skybox
{
    Mesh *skyMesh;
    Shader *skyShader;

    GLuint textureId;
    GLuint uniformProjection;
    GLuint uniformView;

public:
    Skybox();
    Skybox(std::vector<std::string> faceLocations);
    void drawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    ~Skybox(){};
};
