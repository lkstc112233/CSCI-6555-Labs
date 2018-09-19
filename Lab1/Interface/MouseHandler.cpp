#include "MouseHandler.h"

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
	x = xi;
	y = yi;
}

void MouseHandlerContainer::handle() {
	for (auto& handler : handlers) {
		handler.handleEvent(flags, x, y);
	}
	// Reset One-time Flags.
	flags &= ~(
		MOUSE_LEFTBUTTON_PRESSED |
		MOUSE_RIGHTBUTTON_PRESSED |
		MOUSE_SCROLLED_UP_PRESSED |
		MOUSE_SCROLLED_DOWN_PRESSED);
}