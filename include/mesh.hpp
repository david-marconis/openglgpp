#include <GL/glew.h>

class Mesh
{
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
    GLsizei indexCount;

public:
    Mesh();
    ~Mesh();

    void createMesh(GLfloat *vertices, unsigned int *indices, unsigned int vertexCount, unsigned int indexCount);
    void renderMesh();
    void clearMesh();
};
