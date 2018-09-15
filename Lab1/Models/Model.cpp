#include "Model.h"

Model::~Model() {
    delete[] vertexes;
    delete[] indices;
}
