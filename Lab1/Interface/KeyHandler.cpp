#include "KeyHandler.h"

#include <glad/glad.h>

KeyHandler::KeyHandler(GLFWwindow *windowi, int keyListeningi, decltype(handler) handleri, bool pressOnlyi)
	: window(windowi)
	, keyListening(keyListeningi)
	, handler(handleri)
	, pressOnly(pressOnlyi)
{
}

void KeyHandler::handle() {
	if (glfwGetKey(window, keyListening) == GLFW_PRESS) {
		if (!(pressOnly && pressed)) {
			handler(true);
		}
		pressed = true;
	} else {
		if (!pressOnly || pressed) {
			handler(false);
		}
		pressed = false;
	}
}