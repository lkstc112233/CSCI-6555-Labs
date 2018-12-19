#ifndef GRAPHICS_OBJECT_OBJECT_H
#define GRAPHICS_OBJECT_OBJECT_H

#include <initializer_list>
#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include "../../Animate/Timeline.hpp"
#include "../../Math/Quaternion.h"
#include "../Models/Model.h"

#define HANDLE_PROPERTY_IMPLEMENTATION(type, propertyName, functionName)       \
  void set##functionName##Manager(Timeline<type>&& line, float offset = 0) {   \
    managers.emplace(#propertyName,                                            \
                     std::make_unique<ManagerTimeline<type>>(                  \
                         propertyName, std::move(line), offset));              \
  }                                                                            \
  void set##functionName##Manager(std::initializer_list<Timeline<type>> lines, \
                                  float offset = 0) {                          \
    managers.emplace(#propertyName, std::make_unique<ManagerTimeline<type>>(   \
                                        propertyName, lines, offset));         \
  }                                                                            \
  void set##functionName(type propertyValue) { propertyName = propertyValue; } \
  type get##functionName() const { return propertyName; }                      \
  void set##functionName##ManagerRate(float rate) {                            \
    managers[#propertyName]->setRate(rate);                                    \
  }

/**
 * Manages how a model is drawn on screen.
 * Including transcation, rotation, opacity and (if applicable) color.
 */

class Object3D {
 private:
  std::weak_ptr<Object3D> parent;
  const Model& model;
  float centerX = 0;
  float centerY = 0;
  float centerZ = 0;
  float scaleX = 1.0;
  float scaleY = 1.0;
  float scaleZ = 1.0;
  Quaternion orientation;
  float transformX = 0;
  float transformY = 0;
  float transformZ = 0;
  float opacity = 1.0;
  glm::mat4 getTransformationMatrix();
  std::map<std::string, std::unique_ptr<ManagedTimelineInterface>> managers;
  bool absoluteOrientation = false;

 public:
  explicit Object3D(const Model& model);
  void setParent(const std::shared_ptr<Object3D>& parent);
  void updateManagers(float time);
  void moveTo(glm::vec3 position);
  void moveBy(glm::vec3 distance);
  HANDLE_PROPERTY_IMPLEMENTATION(float, transformX, TransformX);
  HANDLE_PROPERTY_IMPLEMENTATION(float, transformY, TransformY);
  HANDLE_PROPERTY_IMPLEMENTATION(float, transformZ, TransformZ);
  HANDLE_PROPERTY_IMPLEMENTATION(Quaternion, orientation, Orientation);
  HANDLE_PROPERTY_IMPLEMENTATION(float, scaleX, ScaleX);
  HANDLE_PROPERTY_IMPLEMENTATION(float, scaleY, ScaleY);
  HANDLE_PROPERTY_IMPLEMENTATION(float, scaleZ, ScaleZ);
  void setScale(float s) {
    setScaleX(s);
    setScaleY(s);
    setScaleZ(s);
  }
  void setCenterX(float cx) { centerX = cx; }
  void setCenterY(float cy) { centerY = cy; }
  void setCenterZ(float cz) { centerZ = cz; }
  void setOpacity(float opacity);
  void setAbsoluteOrientation(bool ao = true) { absoluteOrientation = ao; }
  glm::vec3 getAbsolutePosition();
  void draw(ShaderProgram& shader);
};

#undef SET_TRANSFORM_MANAGER_IMPLEMENTATION

class Object2D {
 private:
  const Model& model;
  glm::mat3 transform;
  glm::vec3 color;
  float z;
  float opacity = 1.0;

 public:
  explicit Object2D(const Model& model);
  void setTransformMatrix(const glm::mat3& transform);
  void setColor(const glm::vec3& color);
  void setR(float r);
  void setG(float g);
  void setB(float b);
  void setZ(float z);
  void setOpacity(float opacity);
  void draw(ShaderProgram& shader);
};

#endif  // GRAPHICS_OBJECT_OBJECT_H