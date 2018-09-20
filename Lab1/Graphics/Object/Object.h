#ifndef GRAPHICS_OBJECT_OBJECT_H
#define GRAPHICS_OBJECT_OBJECT_H

#include <glm/glm.hpp>
#include "../Models/Model.h"

class Object
{
  private:
    const Model &model;

  public:
    Object(const Model &model);
    glm::mat4 transform;
};

#endif // GRAPHICS_OBJECT_OBJECT_H