#ifndef INTERFACE_KEYHANDLER_H
#define INTERFACE_KEYHANDLER_H

#include <vector>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class KeyHandler
{
  private:
	GLFWwindow *window;
	int keyListening;
	// argument: bool: isKeyPressed.
	std::function<void()> handleActive;
	std::function<void()> handleInactive;
	bool pressOnly;
	bool pressed = false;

  public:
	KeyHandler(GLFWwindow *window, int keyListening, decltype(handleActive), bool pressOnly = false, decltype(handleInactive) handleInactive = []() {});
	void handle();
};

class KeyHandlerContainer
{
  private:
	std::vector<KeyHandler> handlers;
};

#endif // INTERFACE_KEYHANDLER_H