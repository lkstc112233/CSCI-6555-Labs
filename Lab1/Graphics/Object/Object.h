#ifndef GRAPHICS_OBJECT_OBJECT_H
#define GRAPHICS_OBJECT_OBJECT_H

#include "../Models/Model.h"
#include <glm/glm.hpp>

class Object
{
  private:
    const Model &model;

  public:
    Object(const Model &model);
    glm::mat4 transform;
};

#endif // GRAPHICS_OBJECT_OBJECT_H