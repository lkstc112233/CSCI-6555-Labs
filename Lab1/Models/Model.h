#ifndef MODELS_MODEL_H
#define MODELS_MODEL_H

#include <glm/glm.hpp>

class Model
{
  private:
    float *vertexes = nullptr;
    int *indices = nullptr;
  public:
    ~Model();
};


#endif // MODELS_MODEL_H