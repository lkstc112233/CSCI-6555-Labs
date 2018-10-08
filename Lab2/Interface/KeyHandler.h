#ifndef INTERFACE_KEYHANDLER_H
#define INTERFACE_KEYHANDLER_H

#include <functional>
#include <vector>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

/**
 * A class managing keyboard operations.
 */
class KeyHandler {
 private:
  GLFWwindow *window;
  int keyListening;
  std::function<void()> handleActive;
  std::function<void()> handleInactive;
  bool pressOnly;
  bool pressed = false;

 public:
  KeyHandler(GLFWwindow *window, int keyListening, decltype(handleActive),
             bool pressOnly = false,
             decltype(handleInactive) handleInactive = []() {});
  void handle();
};

class KeyHandlerContainer {
 private:
  std::vector<KeyHandler> handlers;
  GLFWwindow *window;

 public:
  KeyHandlerContainer(GLFWwindow *window, bool registerMouseLock = true);
  void handle();
  template <class... Args>
  void emplace_handler(Args &&... args) {
    handlers.emplace_back(window, std::forward<Args>(args)...);
  }
};

#endif  // INTERFACE_KEYHANDLER_H