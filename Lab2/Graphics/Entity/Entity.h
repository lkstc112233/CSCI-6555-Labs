#ifndef GRAPHICS_ENTITY_ENTITY_H
#define GRAPHICS_ENTITY_ENTITY_H

#include <map>
#include <memory>
#include <string>

#include "../Object/Object.h"

class Entity {
 private:
  // A map for all the objects in this hierarchical structure.
  std::map<std::string, std::shared_ptr<Object3D>> objects;

 public:
  Entity() {}
  template <class... Args>
  void addObject(std::string name, Args&&... args) {
    objects[name] = std::make_shared<Object3D>(args...);
  }
  template <class... Args>
  void addChild(std::string parent, std::string name, Args&&... args) {
    addObject(name, args...);
    objects[name]->setParent(objects[parent]);
  }
  std::shared_ptr<Object3D> getObject(std::string name) {
    return objects[name];
  }
  void draw(ShaderProgram& shader);
};

#endif  // GRAPHICS_ENTITY_ENTITY_H