#include "MouseHandler.h"

MouseHandler::MouseHandler(std::function<void(int, float, float, float, float)> handler)
	: handleEvent(handler)
{
}

MouseHandler::MouseHandler(std::function<void(int, float, float)> handler)
	: handleEvent([handler](int mouseFlags, float x, float y, float, float) { handler(mouseFlags, x, y); })
{
}

MouseHandler::MouseHandler(std::function<void(int)> handler)
	: handleEvent([handler](int mouseFlags, float, float, float, float) { handler(mouseFlags); })
{
}


MouseHandlerContainer::MouseHandlerContainer(float xi, float yi) 
	: x(xi)
	, y(yi)
	, diffx(0)
	, diffy(0)
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
	x = xi;
	y = yi;
}

void MouseHandlerContainer::handle() {
	for (auto& handler : handlers) {
		handler.handleEvent(flags, x, y, diffx, diffy);
	}
	// Reset One-time Flags.
	flags &= ~(
		MOUSE_LEFTBUTTON_PRESSED |
		MOUSE_RIGHTBUTTON_PRESSED |
		MOUSE_SCROLLED_UP_PRESSED |
		MOUSE_SCROLLED_DOWN_PRESSED);
	diffx = diffy = 0;
}