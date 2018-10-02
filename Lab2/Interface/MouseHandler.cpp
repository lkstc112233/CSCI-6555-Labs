#include "MouseHandler.h"

#include <functional>

#include <glm/glm.hpp>

MouseHandler::MouseHandler(std::function<void(int, float, float, float, float)> handler)
	: handleEvent(handler)
{
}

MouseHandler::MouseHandler(std::function<void(int, float, float)> handler)
	: handleEvent([handler](int mouseFlags, float clampedx, float clampedy, float, float) { handler(mouseFlags, clampedx, clampedy); })
{
}

MouseHandler::MouseHandler(std::function<void(int)> handler)
	: handleEvent([handler](int mouseFlags, float, float, float, float) { handler(mouseFlags); })
{
}


MouseHandlerContainer::MouseHandlerContainer(float xi, float yi, float xClampi, float yClampi)
	: x(xi)
	, y(yi)
	, xClamp(xClampi)
	, yClamp(yClampi)
	, diffx(0)
	, diffy(0)
	, clampedx(glm::clamp(x, -xClampi, xClampi))
	, clampedy(glm::clamp(y, -yClampi, yClampi))
{
}

void MouseHandlerContainer::setScrollUpFlag() {
	flags |= MOUSE_SCROLLED_UP_PRESSED;
}

void MouseHandlerContainer::setScrollDownFlag() {
	flags |= MOUSE_SCROLLED_DOWN_PRESSED;
}

void MouseHandlerContainer::leftPress() {
	flags |= MOUSE_LEFTBUTTON_PRESSED | MOUSE_LEFTBUTTON_HOLD;
}

void MouseHandlerContainer::rightPress() {
	flags |= MOUSE_RIGHTBUTTON_PRESSED | MOUSE_RIGHTBUTTON_HOLD;
}

void MouseHandlerContainer::resetLeftHoldFlag() {
	flags &= ~MOUSE_LEFTBUTTON_HOLD;
}

void MouseHandlerContainer::resetRightHoldFlag() {
	flags &= ~MOUSE_RIGHTBUTTON_HOLD;
}

void MouseHandlerContainer::setPosition(float xi, float yi) {
	diffx += xi - x;
	diffy += yi - y;
	clampedx = glm::clamp(clampedx + diffx, -xClamp, xClamp);
	clampedy = glm::clamp(clampedy - diffy, -yClamp, yClamp); // invert y diff since the mouse uses a different coordinate system.
	x = xi;
	y = yi;
}

void MouseHandlerContainer::handle() {
	for (auto& handler : handlers) {
		handler.handleEvent(flags, clampedx, clampedy, diffx, diffy);
	}
	// Reset One-time Flags.
	flags &= ~(
		MOUSE_LEFTBUTTON_PRESSED |
		MOUSE_RIGHTBUTTON_PRESSED |
		MOUSE_SCROLLED_UP_PRESSED |
		MOUSE_SCROLLED_DOWN_PRESSED);
	diffx = diffy = 0;
}