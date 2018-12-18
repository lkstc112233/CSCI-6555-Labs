#include "Model.h"

#include <fstream>
#include <memory>
#include <vector>

#include <glad/glad.h>

#include "../Shaders/Shader.h"

Model::Model() {
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
}

Model::Model(Model &&model) {
  vertexes = model.vertexes;
  indices = model.indices;
  VAO = model.VAO;
  VBO = model.VBO;
  EBO = model.EBO;
  model.vertexes = nullptr;
  model.indices = nullptr;
}

Model::~Model() {
  delete[] vertexes;
  delete[] indices;
}

void Model::draw(ShaderProgram &shader) const {
  glBindVertexArray(VAO);
  shader.use();
  glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, 0);
}

Model ModelLoader::loadOffFile(const char *filename) {
  Model loadedModel;
  loadedModel.dimensions = 3;
  std::ifstream file(filename);
  std::string type;
  if (!(file >> type)) {
    return loadedModel;
  }

  if (type != "OFF") {
    return loadedModel;
  }

  int vertexesCount;
  int indicesCount;

  file >> vertexesCount >> indicesCount;
  // Drop line since we ignoring line count.
  std::getline(file, type);

  std::vector<float> vertexesWithoutNorm;

  for (int i = 0; i < vertexesCount; ++i) {
    for (int j = 0; j < loadedModel.dimensions; ++j) {
      float coordinate;
      file >> coordinate;
      vertexesWithoutNorm.push_back(coordinate);
    }
  }

  std::vector<unsigned> indices;
  std::vector<float> normWithoutVertexes;
  std::vector<std::pair<unsigned, unsigned>> normIndices;
  for (int i = 0; i < indicesCount; ++i) {
    int count;
    file >> count;
    unsigned firstpoint;
    unsigned pointlast;
    unsigned pointthis;
    file >> firstpoint >> pointlast;
    unsigned normId = normWithoutVertexes.size() / loadedModel.dimensions;
    unsigned normIndicesFirst = normIndices.size();
    normIndices.push_back(std::make_pair(firstpoint, normId));
    unsigned normIndicesLast = normIndices.size();
    normIndices.push_back(std::make_pair(pointlast, normId));
    unsigned normIndicesThis;
    bool normInserted = false;
    for (int j = 0; j < count - 2; ++j) {
      file >> pointthis;
      normIndicesThis = normIndices.size();
      normIndices.push_back(std::make_pair(pointthis, normId));
      // Only calculate norm for each surface with the first 3 vertexes.
      if (!normInserted) {
        normInserted = true;
        // Calculate Norm.
        glm::vec3 point1(
            vertexesWithoutNorm[firstpoint * loadedModel.dimensions],
            vertexesWithoutNorm[firstpoint * loadedModel.dimensions + 1],
            vertexesWithoutNorm[firstpoint * loadedModel.dimensions + 2]);
        glm::vec3 point2(
            vertexesWithoutNorm[pointlast * loadedModel.dimensions],
            vertexesWithoutNorm[pointlast * loadedModel.dimensions + 1],
            vertexesWithoutNorm[pointlast * loadedModel.dimensions + 2]);
        glm::vec3 point3(
            vertexesWithoutNorm[pointthis * loadedModel.dimensions],
            vertexesWithoutNorm[pointthis * loadedModel.dimensions + 1],
            vertexesWithoutNorm[pointthis * loadedModel.dimensions + 2]);
        glm::vec3 vector1 = point1 - point2;
        glm::vec3 vector2 = point1 - point3;
        glm::vec3 normalVector = glm::normalize(glm::cross(vector1, vector2));
        normWithoutVertexes.push_back(normalVector.x);
        normWithoutVertexes.push_back(normalVector.y);
        normWithoutVertexes.push_back(normalVector.z);
      }
      indices.push_back(normIndicesFirst);
      indices.push_back(normIndicesLast);
      indices.push_back(normIndicesThis);
      pointlast = pointthis;
      normIndicesLast = normIndicesThis;
    }
  }
  // Reconstruct vertexes array
  loadedModel.vertexes =
      new float[normIndices.size() * loadedModel.dimensions * 2];
  for (int i = 0; i < normIndices.size(); ++i) {
    for (int j = 0; j < loadedModel.dimensions; ++j) {
      loadedModel.vertexes[i * loadedModel.dimensions * 2 + j] =
          vertexesWithoutNorm[normIndices[i].first * loadedModel.dimensions +
                              j];
    }
    for (int j = 0; j < loadedModel.dimensions; ++j) {
      loadedModel.vertexes[i * loadedModel.dimensions * 2 +
                           loadedModel.dimensions + j] =
          normWithoutVertexes[normIndices[i].second * loadedModel.dimensions +
                              j];
    }
  }

  loadedModel.indices = new unsigned[loadedModel.indicesSize = indices.size()];

  std::copy(begin(indices), end(indices), loadedModel.indices);

  glBindVertexArray(loadedModel.VAO);
  glBindBuffer(GL_ARRAY_BUFFER, loadedModel.VBO);
  glBufferData(GL_ARRAY_BUFFER, normIndices.size() * 6 * sizeof(float),
               loadedModel.vertexes, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedModel.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               loadedModel.indicesSize * sizeof(unsigned), loadedModel.indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  loadedModel.valid = true;
  return loadedModel;
}

Model ModelLoader::loadShpFile(const char *filename) {
  Model loadedModel;
  loadedModel.dimensions = 2;
  std::ifstream file(filename);
  std::string type;
  if (!(file >> type)) {
    return loadedModel;
  }

  if (type != "SHP") {
    return loadedModel;
  }

  int vertexesCount;
  int indicesCount;

  file >> vertexesCount >> indicesCount;

  loadedModel.vertexes = new float[vertexesCount * 2];

  for (int i = 0; i < vertexesCount; ++i) {
    file >> loadedModel.vertexes[i * 2] >> loadedModel.vertexes[i * 2 + 1];
  }

  std::vector<unsigned> indices;
  for (int i = 0; i < indicesCount; ++i) {
    int count;
    file >> count;
    unsigned firstpoint;
    unsigned pointlast;
    unsigned pointthis;
    file >> firstpoint >> pointlast;
    for (int j = 0; j < count - 2; ++j) {
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
  glBufferData(GL_ARRAY_BUFFER, vertexesCount * 2 * sizeof(float),
               loadedModel.vertexes, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loadedModel.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               loadedModel.indicesSize * sizeof(unsigned), loadedModel.indices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  loadedModel.valid = true;
  return loadedModel;
}

const Model &ModelLoader::createModel(std::unique_ptr<Model> &pointerToLoad,
                                      int dimension, const float vertexes[],
                                      int vertexesLength,
                                      const unsigned indices[],
                                      int indicesLength) {
  class UnitSquareShape : public Model {};
  pointerToLoad = std::make_unique<UnitSquareShape>();
  pointerToLoad->valid = true;
  pointerToLoad->dimensions = dimension;
  pointerToLoad->indicesSize = indicesLength / sizeof(unsigned);

  glBindVertexArray(pointerToLoad->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, pointerToLoad->VBO);
  glBufferData(GL_ARRAY_BUFFER, vertexesLength, vertexes, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointerToLoad->EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength, indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, dimension, GL_FLOAT, GL_FALSE,
                        dimension * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
}

std::unique_ptr<Model> ModelLoader::unitSquareShape;
const float UNIT_SQUARE_VERTEXES[] = {0, 0, 0, 1, 1, 0, 1, 1};
const unsigned UNIT_SQUARE_INDICES[] = {0, 1, 3, 0, 3, 2};

std::unique_ptr<Model> ModelLoader::unitTriangleShape;
const float UNIT_TRIANGLE_VERTEXES[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
const unsigned UNIT_TRIANGLE_INDICES[] = {0, 1, 2};

const Model &ModelLoader::getUnitSquareShape() {
  if (!unitSquareShape) {
    createModel(unitSquareShape, 2, UNIT_SQUARE_VERTEXES,
                sizeof(UNIT_SQUARE_VERTEXES), UNIT_SQUARE_INDICES,
                sizeof(UNIT_SQUARE_INDICES));
  }
  return *unitSquareShape;
}

const Model &ModelLoader::getUnitTriangleShape() {
  if (!unitTriangleShape) {
    createModel(unitTriangleShape, 3, UNIT_TRIANGLE_VERTEXES,
                sizeof(UNIT_TRIANGLE_VERTEXES), UNIT_TRIANGLE_INDICES,
                sizeof(UNIT_TRIANGLE_INDICES));
  }
  return *unitTriangleShape;
}
