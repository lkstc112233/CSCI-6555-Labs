#ifndef INTERFACE_UTILITIES_TWOSTATEBUTTON_H
#define INTERFACE_UTILITIES_TWOSTATEBUTTON_H

#include "../../Graphics/Object/Object.h"
#include "../KeyHandler.h"
#include "../MouseHandler.h"

/**
 * A button that has two states that accepts two shapes as state, manages a
 * state bool. The button is located on x, y, x + size, y + size. If the shape
 * model is larger than this, overflowing part will be drawn but not accepting
 * any click. On click the button will trigger and the state will flip. If two
 * buttons overlap, they will both trigger.
 *
 * A rotation can be accepted that will apply to both shapes.
 */
class TwoStateButton {
 private:
  const Model onStateModel;
  const Model offStateModel;
  Object2D onStateShape;
  Object2D offStateShape;
  float x;
  float y;
  float size;

 public:
  TwoStateButton(const char* shapeOn, const char* shapeOff, float x, float y,
                 float size, float rotation = 0);
  bool state = true;
  void attachControls(MouseHandlerContainer& mouseContainer);
  void draw(ShaderProgram& shader);
};

#endif  // INTERFACE_UTILITIES_TWOSTATEBUTTON_H