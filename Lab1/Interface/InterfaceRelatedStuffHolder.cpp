#include "InterfaceRelatedStuffHolder.hpp"

#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MouseHandler.h"

MouseHandlerContainer *MouseCallbackWrapper::handlers = nullptr;

void attachCameraControls(KeyHandlerContainer& keyContainer, MouseHandlerContainer& mouseContainer, Camera &camera) {
	keyContainer.emplace_handler(GLFW_KEY_W, [&camera]() {
		camera.moveForward();
	});
	keyContainer.emplace_handler(GLFW_KEY_S, [&camera]() {
		camera.moveBackward();
	});
	keyContainer.emplace_handler(GLFW_KEY_A, [&camera]() {
		camera.moveLeft();
	});
	keyContainer.emplace_handler(GLFW_KEY_D, [&camera]() {
		camera.moveRight();
	});
	keyContainer.emplace_handler(GLFW_KEY_R, [&camera]() {
		camera.moveUp();
	});
	keyContainer.emplace_handler(GLFW_KEY_F, [&camera]() {
		camera.moveDown();
	});
	mouseContainer.emplace_handler([&camera](int mouseFlags, float, float, float diffx, float diffy) {
		if (mouseFlags & MOUSE_RIGHTBUTTON_HOLD) {
			float sensitivity = 0.005f;
			diffx *= sensitivity;
			diffy *= sensitivity;

			camera.turnYaw(diffx);
			camera.turnPitch(diffy);
		}
	});
}