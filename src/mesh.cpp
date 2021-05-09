#include "mesh.hpp"

Mesh::Mesh()
{
    vao = 0;
    vbo = 0;
    ibo = 0;
}

void Mesh::createMesh(
    GLfloat *vertices,
    unsigned int *indices,
    unsigned int vertexCount,
    unsigned int indexCount)
{
    this->indexCount = indexCount;

    glCreateVertexArrays(1, &vao);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao, 0, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
    glVertexArrayAttribBinding(vao, 1, 0);

    glEnableVertexArrayAttrib(vao, 2);
    glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5);
    glVertexArrayAttribBinding(vao, 2, 0);

    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(GLfloat) * vertexCount, vertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(GLfloat) * 8);

    glCreateBuffers(1, &ibo);
    glNamedBufferData(ibo, sizeof(unsigned int) * indexCount, indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vao, ibo);
}

void Mesh::renderMesh()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::clearMesh()
{
    if (ibo != 0)
    {
        glDeleteBuffers(1, &ibo);
        ibo = 0;
    }
    if (vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    indexCount = 0;
}

Mesh::~Mesh()
{
    clearMesh();
}
