#include "InterfaceRelatedStuffHolder.hpp"

#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MouseHandler.h"

MouseHandlerContainer *MouseCallbackWrapper::handlers = nullptr;