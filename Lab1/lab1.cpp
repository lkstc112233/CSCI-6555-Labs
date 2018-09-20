// standard
#include <cassert>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/WindowInitializer.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/Models/Model.h"
#include "Graphics/Object/Object.h"

#include "Interface/KeyHandler.h"
#include "Interface/MouseHandler.h"
#include "Interface/InterfaceRelatedStuffHolder.hpp"

#include "Math/Quaternion.h"
#include "Animate/Keyframe.hpp"
#include "Animate/Interpolate.hpp"
#include "Animate/Scripts.h"

const float PROJECTION_RATIO = float(SCREEN_WIDTH) / SCREEN_HEIGHT;

void update()
{
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		std::cerr << "Usage:\n\t" << argv[0] << " *model.off* *script.keys*" << std::endl;
		return -1;
	}

	auto script = ScriptsLoader::loadScript(argv[2]);

	GLFWwindow *window = initializeWindow("Lab1");
	if (!window) {
		return -1;
	}

	double mouseX, mouseY;
	int windowWidth, windowHeight;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	MouseHandlerContainer mouseHandlers(mouseX, mouseY, windowWidth, windowHeight);
	MouseCallbackWrapper::registerHandlerCallbacks(window, &mouseHandlers);

	Object3D cube(ModelLoader::loadOffFile(argv[1]));

	ShaderProgram shaderProgram{
		Shader::createVertexShader("res/shaders/simpleShader.vert"),
		Shader::createFragmentShader("res/shaders/simpleShader.frag")};
	if (!shaderProgram.isValid())
	{
		return -4;
	}
	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), PROJECTION_RATIO, 0.1f, 100.0f);
	shaderProgram.setMatrix("projection", projection);

	Camera camera;

	KeyHandlerContainer keyHandlers(window);

	attachCameraControls(keyHandlers, mouseHandlers, camera);

	keyHandlers.emplace_handler(GLFW_KEY_ESCAPE, [window]() {
		glfwSetWindowShouldClose(window, true);
	});

	ShaderProgram playPauseShader{
		Shader::createVertexShader("res/shaders/2DShader.vert"),
		Shader::createFragmentShader("res/shaders/2DShader.frag")};
	bool playing = true;

	Object2D cursor(ModelLoader::loadShpFile("res/shapes/cursor.shp"));
	ShaderProgram cursorShader{
		Shader::createVertexShader("res/shaders/2DShader.vert"),
		Shader::createFragmentShader("res/shaders/2DShader.frag")};
	if (!cursorShader.isValid())
	{
		return -4;
	}
	mouseHandlers.emplace_handler([&cursor, &cursorShader, &playing](int mouseFlags, float clampedx, float clampedy) {
		glm::mat3 cursorTransform(1.0f);
		cursorTransform[2] = { clampedx / SCREEN_WIDTH, clampedy / SCREEN_HEIGHT, 1.0};
		cursor.setTransformMatrix(cursorTransform);
		if (mouseFlags & MOUSE_RIGHTBUTTON_HOLD) {
			cursorShader.setVector("color", glm::vec4(glm::vec3(1.0f), 0.0f));
		} else {
			cursorShader.setVector("color", glm::vec4(1.0f));
		}
	}); 
	Object2D play(ModelLoader::loadShpFile("res/shapes/play.shp"));
	Object2D pause(ModelLoader::loadShpFile("res/shapes/pause.shp"));
	glm::mat3 buttonTransform{{0.1, 0, 0}, {0, 0.1, 0}, {-0.95, -0.95, 1}};
	play.setTransformMatrix(buttonTransform);
	pause.setTransformMatrix(buttonTransform);
	mouseHandlers.emplace_handler([&playing](int mouseFlags, float clampedx, float clampedy) {
		float x = clampedx / SCREEN_WIDTH;
		float y = clampedy / SCREEN_HEIGHT;
		if ((mouseFlags & MOUSE_LEFTBUTTON_PRESSED) && x < -0.9 && y < -0.9) {
			playing = !playing;
		}
	});

	double acumulatedTime = 0;
	double lastTime = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		keyHandlers.handle();
		mouseHandlers.handle();

		double thisTime = glfwGetTime();
		if (playing) {
			acumulatedTime += thisTime - lastTime;
		}
		lastTime = thisTime;

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cube.setTransformMatrix(script->getTranscationMatrixAt(acumulatedTime));

		shaderProgram.setMatrix("view", camera.getViewMat());
		cube.draw(shaderProgram);

		// Draw HDR
		glClear(GL_DEPTH_BUFFER_BIT);
		playPauseShader.setValue("z", 0);
		if (playing) {
			pause.draw(playPauseShader);
		} else {
			play.draw(playPauseShader);
		}

		// Draw cursor
		glClear(GL_DEPTH_BUFFER_BIT);
		cursor.draw(cursorShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}