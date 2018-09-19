#ifndef INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP
#define INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MouseHandler.h"

class MouseCallbackWrapper
{
  private:
	static MouseHandlerContainer *handlers;

  public:
    static void registerHandlerCallbacks(GLFWwindow *window, MouseHandlerContainer *handler) {
		handlers = handler;
		glfwSetCursorPosCallback(window, callback);
		glfwSetMouseButtonCallback(window, callback);
	}
	static void callback(GLFWwindow *window, double x, double y)
	{
		if (handlers)
		{
			handlers->setPosition(x, y);
		}
	}
	static void callback(GLFWwindow *window, int button, int action, int mods)
	{
		if (handlers)
		{
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
				if (action == GLFW_PRESS)
				{
					handlers->leftPress();
				}
				else
				{
					handlers->resetLeftHoldFlag();
				}
				break;
			case GLFW_MOUSE_BUTTON_RIGHT:
				if (action == GLFW_PRESS)
				{
					handlers->rightPress();
				}
				else
				{
					handlers->resetRightHoldFlag();
				}
				break;
			}
		}
	}
};

#endif // INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP
