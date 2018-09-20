#ifndef GRAPHICS_MODELS_MODEL_H
#define GRAPHICS_MODELS_MODEL_H

#include <glm/glm.hpp>

#include "../Shaders/Shader.h"

class Model
{
private:
  friend class ModelLoader;
  float *vertexes = nullptr;
  int dimensions = 0;
  unsigned *indices = nullptr;
  int indicesSize = 0;
  bool valid = true;
  unsigned VAO = 0, VBO = 0, EBO = 0;
  Model();
  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;

public:
  ~Model();
  Model(Model &&);
  void draw(ShaderProgram &shader);
  int getDimensions() { return dimensions; }
  bool isValid() { return valid; }
};

class ModelLoader
{
public:
  static Model loadOffFile(const char *filename);
  static Model loadShpFile(const char *filename);
};

#endif // GRAPHICS_MODELS_MODEL_H