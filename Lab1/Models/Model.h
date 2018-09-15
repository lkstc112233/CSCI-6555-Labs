#ifndef MODELS_MODEL_H
#define MODELS_MODEL_H

#include <glm/glm.hpp>

class Model
{
  private:
    friend class ModelLoader;
    float *vertexes = nullptr;
    int *indices = nullptr;
    bool valid = true;
    unsigned VAO, VBO, EBO;
    Model();
    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;

  public:
    ~Model();
    Model(Model &&);
    bool isValid() { return valid; }
};

class ModelLoader
{
  public:
    static Model loadFile(const char *filename);
};

#endif // MODELS_MODEL_H