#ifndef INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP
#define INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP

#include <functional>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../Graphics/Camera/Camera.h"
#include "KeyHandler.h"
#include "MouseHandler.h"

void attachCameraControls(KeyHandlerContainer &keyContainer,
                          MouseHandlerContainer &mouseContainer,
                          Camera &camera);

class MouseCallbackWrapper {
 private:
  constexpr const static double SCROLL_THRESHOLD = 1;
  constexpr const static double SCROLL_OFFSET_FADING_CONSTANT = 0.997;
  static MouseHandlerContainer *handlers;
  static double yoffset;

 public:
  static void registerHandlerCallbacks(GLFWwindow *window,
                                       MouseHandlerContainer *handler) {
    handlers = handler;
    glfwSetCursorPosCallback(window, callback);
    glfwSetMouseButtonCallback(window, callback);
    glfwSetScrollCallback(window, callback_scroll);
  }
  static void registerMouseTriggerCallback(GLFWwindow *window,
                                           KeyHandlerContainer &handler);
  static void callback(GLFWwindow *, double x, double y) {
    if (handlers) {
      handlers->setPosition(x, y);
    }
  }
  static void callback_scroll(GLFWwindow *, double, double diffy) {
    if (yoffset * diffy < 0) {
      yoffset = 0;
    }
    yoffset += diffy;
    yoffset *= SCROLL_OFFSET_FADING_CONSTANT;
    if (handlers) {
      if (yoffset > SCROLL_THRESHOLD) {
        yoffset -= SCROLL_THRESHOLD;
        handlers->setScrollUpFlag();
      } else if (yoffset < -SCROLL_THRESHOLD) {
        yoffset += SCROLL_THRESHOLD;
        handlers->setScrollDownFlag();
      }
    }
  }
  static void callback(GLFWwindow *, int button, int action, int) {
    if (handlers) {
      if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
          handlers->leftPress();
        } else {
          handlers->resetLeftHoldFlag();
        }
      } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
          handlers->rightPress();
        } else {
          handlers->resetRightHoldFlag();
        }
      }
    }
  }
};

#endif  // INTERFACE_INTERFACERELATEDSTUFFHOLDER_HPP
