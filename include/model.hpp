#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "texture.hpp"

class Model
{
    std::string modelLocation;
    std::vector<Mesh *> meshes;
    std::vector<Texture *> textures;
    std::vector<unsigned int> meshToTexture;

    void loadNode(aiNode *node, const aiScene *scene);
    void loadMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterials(const aiScene *scene);

public:
    Model(std::string modelLocation) : modelLocation(modelLocation){};
    ~Model(){};

    void loadModel(const std::string &filename);
    void renderModel();
    void clearModel();
};
