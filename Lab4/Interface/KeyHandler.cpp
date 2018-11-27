#include "KeyHandler.h"

#include <glad/glad.h>

#include "InterfaceRelatedStuffHolder.hpp"

KeyHandler::KeyHandler(GLFWwindow *windowi, int keyListeningi,
                       decltype(handleActive) handleri, bool pressOnlyi,
                       decltype(handleInactive) handleInactivei)
    : window(windowi),
      keyListening(keyListeningi),
      handleActive(handleri),
      handleInactive(handleInactivei),
      pressOnly(pressOnlyi) {}

void KeyHandler::handle() {
  if (glfwGetKey(window, keyListening) == GLFW_PRESS) {
    if (!(pressOnly && pressed)) {
      handleActive();
    }
    pressed = true;
  } else {
    if (!pressOnly || pressed) {
      handleInactive();
    }
    pressed = false;
  }
}

KeyHandlerContainer::KeyHandlerContainer(GLFWwindow *windowi,
                                         bool registerMouseLock)
    : window(windowi) {
  if (registerMouseLock) {
    MouseCallbackWrapper::registerMouseTriggerCallback(window, *this);
  }
}

void KeyHandlerContainer::handle() {
  for (auto &handler : handlers) {
    handler.handle();
  }
}