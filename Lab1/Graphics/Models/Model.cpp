#include "Model.h"

#include <vector>
#include <fstream>

#include <glad/glad.h>

#include "../Shaders/Shader.h"

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

void Model::draw(ShaderProgram &shader)
{
    glBindVertexArray(VAO);
    shader.use();
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

Model ModelLoader::loadOffFile(const char *filename)
{
    Model loadedModel;
    std::ifstream file(filename);
    std::string type;
    if (!(file >> type))
    {
        loadedModel.valid = false;
        return loadedModel;
    }

    if (type != "OFF")
    {
        loadedModel.valid = false;
        return loadedModel;
    }

    int vertexesCount;
    int indicesCount;

    file >> vertexesCount >> indicesCount;
    // Drop line since we ignoring line count.
    std::getline(file, type);

    loadedModel.vertexes = new float[vertexesCount * 3];

    for (int i = 0; i < vertexesCount; ++i)
    {
        file >> loadedModel.vertexes[i * 3] >> loadedModel.vertexes[i * 3 + 1] >> loadedModel.vertexes[i * 3 + 2];
    }

    std::vector<unsigned> indices;
    for (int i = 0; i < indicesCount; ++i)
    {
        int count;
        file >> count;
        unsigned firstpoint;
        unsigned pointlast;
        unsigned pointthis;
        file >> firstpoint >> pointlast;
        for (int j = 0; j < count - 2; ++j)
        {
            file >> pointthis;
            indices.push_back(firstpoint);
            indices.push_back(pointlast);
            indices.push_back(pointthis);
            pointlast = pointthis;
        }
    }
    loadedModel.indices = new unsigned[loadedModel.indicesSize = indices.size()];

    std::copy(begin(indices), end(indices), loadedModel.indices);

    glBindVertexArray(loadedModel.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, loadedModel.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexesCount * 3 * sizeof(float), loadedModel.vertexes, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedModel.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedModel.indicesSize * sizeof(unsigned), loadedModel.indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    return loadedModel;
}
