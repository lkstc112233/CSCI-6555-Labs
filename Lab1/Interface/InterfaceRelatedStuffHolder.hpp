#ifndef INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP
#define INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MouseHandler.h"

class MouseCallbackWrapper {
public:
	static MouseHandlerContainer* handler;
	static void callback(GLFWwindow *window, double x, double y) {
		if (handler) {
			handler->setPosition(x, y);
		}
	}
	static void callback(GLFWwindow* window, int button, int action, int mods) {
		if (handler) {
			switch (button) {
			case GLFW_MOUSE_BUTTON_RIGHT:
				if (action == GLFW_PRESS) {
					handler->leftPress();
				} else {
					handler->resetLeftHoldFlag();
				}
				break;
			case GLFW_MOUSE_BUTTON_LEFT:
				if (action == GLFW_PRESS) {
					handler->rightPress();
				}
				else {
					handler->resetRightHoldFlag();
				}
				break;
			}
		}
	}
};

#endif // INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP
