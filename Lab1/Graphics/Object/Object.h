#ifndef GRAPHICS_OBJECT_OBJECT_H
#define GRAPHICS_OBJECT_OBJECT_H

#include <glm/glm.hpp>
#include "../Models/Model.h"

class Object3D
{
  private:
    const Model &model;
	glm::mat4 transform;

  public:
	Object3D(const Model &model);
	void setTransformMatrix(const glm::mat4& transform);
	void draw(ShaderProgram& shader);
};

#endif // GRAPHICS_OBJECT_OBJECT_H