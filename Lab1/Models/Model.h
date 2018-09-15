#ifndef MODELS_MODEL_H
#define MODELS_MODEL_H

#include <glm/glm.hpp>

class Model {
private:
    float *vertexes = 0;
    int *indices = 0;
public:
    ~Model();
};


#endif // MODELS_MODEL_H