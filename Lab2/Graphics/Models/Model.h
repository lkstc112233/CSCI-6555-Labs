#ifndef GRAPHICS_MODELS_MODEL_H
#define GRAPHICS_MODELS_MODEL_H

#include <memory>
#include <glm/glm.hpp>

#include "../Shaders/Shader.h"

/**
 * Manages a model, including VAO, VBO, and (if appliable) EBO.
 */
class Model
{
private:
  friend class ModelLoader;
  float *vertexes = nullptr;
  int dimensions = 0;
  unsigned *indices = nullptr;
  int indicesSize = 0;
  bool valid = false;
  unsigned VAO = 0;
  unsigned VBO = 0;
  unsigned EBO = 0;
  Model();
  Model(const Model &) = delete;
  Model &operator=(const Model &) = delete;

public:
  ~Model();
  Model(Model &&);
  void draw(ShaderProgram &shader) const;
  int getDimensions() const { return dimensions; }
  bool isValid() const { return valid; }
};

class ModelLoader
{
private:
  static std::unique_ptr<Model> unitSquareShape;
public:
  static const Model& getUnitSquareShape();
  static Model loadOffFile(const char *filename);
  static Model loadShpFile(const char *filename);
};

#endif // GRAPHICS_MODELS_MODEL_H