#include <iostream>

#include "model.hpp"

void Model::loadModel(const std::string &filename)
{
    std::string filePath = modelLocation + filename;
    Assimp::Importer importer;
    unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices;
    const aiScene *scene = importer.ReadFile(filePath, flags);
    if (!scene)
    {
        std::cerr << "Model " << filePath << "failed to load: " << importer.GetErrorString() << std::endl;
        return;
    }
    loadNode(scene->mRootNode, scene);
    loadMaterials(scene);
}

void Model::loadNode(aiNode *node, const aiScene *scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        loadNode(node->mChildren[i], scene);
    }
}

void Model::loadMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.insert(vertices.end(), {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z});

        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
        }
        else
        {
            vertices.insert(vertices.end(), {0.0f, 0.0f});
        }
        vertices.insert(vertices.end(), {-mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z});
    }
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    Mesh *newMesh = new Mesh();
    newMesh->createMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
    meshes.push_back(newMesh);
    meshToTexture.push_back(mesh->mMaterialIndex);
}

void Model::loadMaterials(const aiScene *scene)
{
    textures.resize(scene->mNumMaterials);
    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial *material = scene->mMaterials[i];
        textures[i] = nullptr;
        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                int idx = std::string(path.data).rfind('\\');
                std::string filename = std::string(path.data).substr(idx + 1);
                std::string texturePath = modelLocation + filename;
                textures[i] = new Texture(texturePath.c_str());
                if (!textures[i]->loadTexture())
                {
                    std::cerr << "Failed to load texture from file: " << texturePath << std::endl;
                    delete textures[i];
                    textures[i] = nullptr;
                }
            }
        }
        if (!textures[i])
        {
            textures[i] = new Texture("resources/plain.png");
            textures[i]->loadTextureWithAlpha();
        }
    }
}

void Model::renderModel()
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        unsigned int materialIndex = meshToTexture[i];
        if (materialIndex < textures.size() && textures[materialIndex])
        {
            textures[materialIndex]->useTexture();
        }
        meshes[i]->renderMesh();
    }
}

void Model::clearModel()
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        if (meshes[i])
        {
            delete meshes[i];
            meshes[i] = nullptr;
        }
    }
    for (size_t i = 0; i < textures.size(); i++)
    {
        if (textures[i])
        {
            delete textures[i];
            textures[i] = nullptr;
        }
    }
}
