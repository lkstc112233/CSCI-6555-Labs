#ifndef INTERFACE_KEYHANDLER_H
#define INTERFACE_KEYHANDLER_H

#include <functional>

class KeyHandler {
private:
	GLFWwindow *window;
	int keyListening;
	// argument: bool: isKeyPressed.
	std::function<void(bool)> handler;
	bool pressOnly;
	bool pressed = false;
public:
	KeyHandler(GLFWwindow *window, int keyListening, decltype(handler), bool pressOnly = false);
	void handle();
};

#endif // INTERFACE_KEYHANDLER_H