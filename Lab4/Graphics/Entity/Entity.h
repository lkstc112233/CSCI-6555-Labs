#ifndef GRAPHICS_ENTITY_ENTITY_H
#define GRAPHICS_ENTITY_ENTITY_H

#include <map>
#include <memory>
#include <string>

#include "../Object/Object.h"

class Entity {
 private:
  typedef std::map<std::string, std::shared_ptr<Object3D>> MapType;
  // A map for all the objects in this hierarchical structure.
  MapType objects;

 public:
  class iterator {
   private:
    MapType::iterator m_iterator;
    friend class Entity;

   public:
    iterator& operator++() {
      ++m_iterator;
      return *this;
    }
    iterator operator++(int) {
      auto temp = *this;
      m_iterator++;
      return temp;
    }
    iterator& operator--() {
      --m_iterator;
      return *this;
    }
    iterator operator--(int) {
      auto temp = *this;
      m_iterator--;
      return temp;
    }
    bool operator==(const iterator& another) const {
      return m_iterator == another.m_iterator;
    }
    bool operator!=(const iterator& another) const {
      return m_iterator != another.m_iterator;
    }
    std::shared_ptr<Object3D> operator*() { return m_iterator->second; }
    Object3D* operator->() { return m_iterator->second.operator->(); }
  };
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
  void updateManagers(float time);
  void draw(ShaderProgram& shader);
  iterator begin() {
    iterator i;
    i.m_iterator = objects.begin();
    return i;
  }
  iterator end() {
    iterator i;
    i.m_iterator = objects.end();
    return i;
  }
};

#endif  // GRAPHICS_ENTITY_ENTITY_H