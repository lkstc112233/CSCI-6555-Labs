#include "Model.h"

#include <glad/glad.h>

Model::Model()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

Model::Model(Model &&model)
{
    vertexes = model.vertexes;
    indices = model.indices;
    VAO = model.VAO;
    VBO = model.VBO;
    EBO = model.EBO;
    model.vertexes = nullptr;
    model.indices = nullptr;
}

Model::~Model()
{
    delete[] vertexes;
    delete[] indices;
}

Model ModelLoader::loadFile(const char *filename)
{
}
