#ifndef INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP
#define INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Holder {
public:
	static std::function<void(GLFWwindow *window, double width, double height)> mouseCallback;
	static void callback(GLFWwindow *window, double width, double height) {
		if (mouseCallback) mouseCallback(window, width, height);
	}
};

#endif // INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP
