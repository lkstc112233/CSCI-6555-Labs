#ifndef GRAPHICS_OBJECT_OBJECT_H
#define GRAPHICS_OBJECT_OBJECT_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include "../../Animate/Timeline.hpp"
#include "../../Math/Quaternion.h"
#include "../Models/Model.h"

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
  Quaternion orientation;
  float transformX = 0;
  float transformY = 0;
  float transformZ = 0;
  float opacity = 1.0;
  glm::mat4 getTransformationMatrix();
  std::vector<std::unique_ptr<ManagedTimelineInterface>> managers;

 public:
  explicit Object3D(const Model& model);
  void setParent(const std::shared_ptr<Object3D>& parent);
  void updateManagers(float time);
  void setCenterX(float cx) { centerX = cx; }
  void setCenterY(float cy) { centerY = cy; }
  void setCenterZ(float cz) { centerZ = cz; }
  void setTransformX(float tx) { transformX = tx; }
  void setTransformY(float ty) { transformY = ty; }
  void setTransformZ(float tz) { transformZ = tz; }
  void setOrientation(Quaternion q) { orientation = q; }
  void setOpacity(float opacity);
  void draw(ShaderProgram& shader);
};

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