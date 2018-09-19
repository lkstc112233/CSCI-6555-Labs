#include "InterfaceRelatedStuffHolder.hpp"

#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::function<void(GLFWwindow *window, double width, double height)> Holder::mouseCallback;