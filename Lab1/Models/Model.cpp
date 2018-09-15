#include "Model.h"

#include <vector>
#include <fstream>

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
    Model loadedModel;
	std::ifstream file(filename);
	std::string type;
	if(!(file >> type)) {
		loadedModel.valid = false;
		return loadedModel;
	}

    if (type != "OFF") {
        loadedModel.valid = false;
        return loadedModel;
    }

    int vertexesCount;
    int indicesCount;

    file >> vertexesCount >> indicesCount;
    // Drop line since we ignoring line count.
    std::getline(file, type);

    loadedModel.vertexes = new float[vertexesCount * 3];

    for (int i = 0; i < vertexesCount; ++i) {
        file >> loadedModel.vertexes[i * 3] >> loadedModel.vertexes[i * 3 + 1] >> loadedModel.vertexes[i * 3 + 2];
    }

    std::vector<int> indices;
    for (int i = 0; i < indicesCount; ++i) {
        int count;
        file >> count;
        int firstpoint;
        int pointlast;
        int pointthis;
        file >> firstpoint >> pointlast;
        for (int j = 0; j < count - 2; ++j) {
            file >> pointthis;
            indices.push_back(firstpoint);
            indices.push_back(pointlast);
            indices.push_back(pointthis);
            pointlast = pointthis;
        }
    }
    loadedModel.indices = new int[indices.size()];

    std::copy(begin(indices), end(indices), loadedModel.indices);

	glBindVertexArray(loadedModel.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, loadedModel.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexesCount * 3, loadedModel.vertexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedModel.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), loadedModel.indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

    return loadedModel;
}
