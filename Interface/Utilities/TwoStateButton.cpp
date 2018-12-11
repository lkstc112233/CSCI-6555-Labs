#include "TwoStateButton.h"

#include <cmath>

#include "../../Graphics/Object/Object.h"
#include "../../Graphics/WindowInitializer.h"

TwoStateButton::TwoStateButton(const char* shapeOn, const char* shapeOff,
                               float xi, float yi, float sizei, float rotation)
    : onStateShape(onStateModel),
      offStateShape(offStateModel),
      onStateModel(ModelLoader::loadShpFile(shapeOn)),
      offStateModel(ModelLoader::loadShpFile(shapeOff)),
      x(xi),
      y(yi),
      size(sizei) {
  glm::mat3 buttonTransform{{size * cos(rotation), size * sin(rotation), 0},
                            {-size * sin(rotation), size * cos(rotation), 0},
                            {x + size / 2, y + size / 2, 1}};
  onStateShape.setTransformMatrix(buttonTransform);
  offStateShape.setTransformMatrix(buttonTransform);
}

void TwoStateButton::attachControls(MouseHandlerContainer& mouseContainer) {
  mouseContainer.emplace_handler(
      [this](int mouseFlags, float clampedx, float clampedy) {
        float xc = clampedx / SCREEN_WIDTH;
        float yc = clampedy / SCREEN_HEIGHT;
        if ((mouseFlags & MOUSE_LEFTBUTTON_PRESSED) && xc < x + size &&
            xc > x && yc < y + size && yc > y) {
          state = !state;
        }
      });
}

void TwoStateButton::draw(ShaderProgram& shader) {
  if (state) {
    onStateShape.draw(shader);
  } else {
    offStateShape.draw(shader);
  }
}