#ifndef INTERFACE_UTILITIES_BUTTON_H
#define INTERFACE_UTILITIES_BUTTON_H

#include <functional>

#include "../../Graphics/Object/Object.h"
#include "../KeyHandler.h"
#include "../MouseHandler.h"

/**
 * Manages a button on screen.
 */
class Button {
private:
    const Model model;
	Object2D shape;
    float x;
    float y;
    float size;
    std::function<void()> click;
public:
    Button(const char* shapeOn, float x, float y, float size, float rotation = 0);
    void setCallback(std::function<void()> callback) { click = callback; }
    void attachControls(MouseHandlerContainer& mouseContainer);
	void draw(ShaderProgram& shader);
};

#endif // INTERFACE_UTILITIES_BUTTON_H