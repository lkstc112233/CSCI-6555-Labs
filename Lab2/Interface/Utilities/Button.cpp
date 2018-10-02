#include "Button.h"

#include <cmath>

#include "../../Graphics/WindowInitializer.h"
#include "../../Graphics/Object/Object.h"

Button::Button(const char* shapei, float xi, float yi, float sizei, float rotation)
    : shape(model)
    , model(ModelLoader::loadShpFile(shapei))
    , x(xi)
    , y(yi)
    , size(sizei)
{
	glm::mat3 buttonTransform {
        {size * cos(rotation), size * sin(rotation), 0},
        {-size * sin(rotation), size * cos(rotation), 0}, 
        {x + size / 2, y + size / 2, 1}};
	shape.setTransformMatrix(buttonTransform);
}

void Button::attachControls(MouseHandlerContainer& mouseContainer) {
    mouseContainer.emplace_handler([this](int mouseFlags, float clampedx, float clampedy) {
		float xc = clampedx / SCREEN_WIDTH;
		float yc = clampedy / SCREEN_HEIGHT;
		if ((mouseFlags & MOUSE_LEFTBUTTON_PRESSED) 
            && xc < x + size 
            && xc > x 
            && yc < y + size 
            && yc > y) {
            if (click) {
                click();
            }
		}
	});
}

void Button::draw(ShaderProgram &shader) {
    shape.draw(shader);
}