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

void Model::draw(ShaderProgram &shader) const
{
    glBindVertexArray(VAO);
    shader.use();
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

Model ModelLoader::loadOffFile(const char *filename)
{
    Model loadedModel;
	loadedModel.dimensions = 3;
    std::ifstream file(filename);
    std::string type;
    if (!(file >> type))
    {
        return loadedModel;
    }

    if (type != "OFF")
    {
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

	loadedModel.valid = true;
    return loadedModel;
}

Model ModelLoader::loadShpFile(const char *filename)
{
    Model loadedModel;
	loadedModel.dimensions = 2;
    std::ifstream file(filename);
    std::string type;
    if (!(file >> type))
    {
        return loadedModel;
    }

    if (type != "SHP")
    {
        return loadedModel;
    }

    int vertexesCount;
    int indicesCount;

    file >> vertexesCount >> indicesCount;

    loadedModel.vertexes = new float[vertexesCount * 2];

    for (int i = 0; i < vertexesCount; ++i)
    {
        file >> loadedModel.vertexes[i * 2] >> loadedModel.vertexes[i * 2 + 1];
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
    glBufferData(GL_ARRAY_BUFFER, vertexesCount * 2 * sizeof(float), loadedModel.vertexes, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedModel.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadedModel.indicesSize * sizeof(unsigned), loadedModel.indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

	loadedModel.valid = true;
    return loadedModel;
}

Model ModelLoader::unitSquareShape;
const float UNIT_SQUARE_VERTEXES[] =
{
	0, 0,
	0, 1,
	1, 0,
	1, 1
};
const unsigned UNIT_SQUARE_INDICES[] =
{
	0, 1, 3,
	0, 3, 2,
};

const Model& ModelLoader::getUnitSquareShape()
{
	if (unitSquareShape.isValid()) {
		return unitSquareShape;
	}
	unitSquareShape.valid = true;
	unitSquareShape.dimensions = 2;
	int indicesCount = 1;

	glBindVertexArray(unitSquareShape.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, unitSquareShape.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UNIT_SQUARE_VERTEXES), UNIT_SQUARE_VERTEXES, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, unitSquareShape.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UNIT_SQUARE_INDICES), UNIT_SQUARE_INDICES, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	return unitSquareShape;
}
