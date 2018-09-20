// standard
#include <cassert>
#include <cmath>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Graphics/Shaders/Shader.h"
#include "Graphics/Texture/Texture.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/Models/Model.h"

#include "Interface/KeyHandler.h"
#include "Interface/MouseHandler.h"
#include "Interface/InterfaceRelatedStuffHolder.hpp"

#include "Math/Quaternion.h"
#include "Animate/Keyframe.hpp"
#include "Animate/Interpolate.hpp"
#include "Animate/Scripts.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float PROJECTION_RATIO = float(SCREEN_WIDTH) / SCREEN_HEIGHT;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void update()
{
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		std::cerr << "Usage:\n\t" << argv[0] << " *model.off* *script.keys*" << std::endl;
	}

	auto script = ScriptsLoader::loadScript(argv[2]);

	if (!glfwInit())
	{
		std::cerr << "GLFW initialize failed" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Error occurred creating window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "GLAD initialize failed" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
	MouseHandlerContainer mouseHandlers(mouseX, mouseY);
	MouseCallbackWrapper::registerHandlerCallbacks(window, &mouseHandlers);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Model cube = ModelLoader::loadOffFile(argv[1]);
	if (!cube.isValid()) {
		std::cerr << "Loading model file '" << argv[1] << "' failed." << std::endl;
		return -1;
	}

	ShaderProgram shaderProgram{
		Shader::createVertexShader("res/shaders/simpleShader.vert"),
		Shader::createFragmentShader("res/shaders/simpleShader.frag")};
	if (!shaderProgram.isValid())
	{
		return -4;
	}
	shaderProgram.setMatrix("transform", glm::mat4(1.0f));

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
	glm::mat3 buttonTransform(1.0f);
	buttonTransform[0][0] = 0.1;
	buttonTransform[1][1] = 0.1;
	buttonTransform[2][0] = -0.95;
	buttonTransform[2][1] = -0.95;
	playPauseShader.setVector("color", glm::vec4(1.0f));
	playPauseShader.setMatrix("transform", buttonTransform);
	bool playing = true;

	ShaderProgram cursorShader{
		Shader::createVertexShader("res/shaders/2DShader.vert"),
		Shader::createFragmentShader("res/shaders/2DShader.frag")};
	if (!cursorShader.isValid())
	{
		return -4;
	}
	glm::mat3 cursorTransform(1.0f);
	cursorTransform[2][0] = mouseX / SCREEN_WIDTH - 1;
	cursorTransform[2][1] = mouseY / SCREEN_HEIGHT - 1;
	cursorShader.setVector("color", glm::vec4(1.0f));
	cursorShader.setMatrix("transform", cursorTransform);
	mouseHandlers.emplace_handler([&cursorTransform, &cursorShader, &playing](int mouseFlags, float diffx, float diffy) {
		cursorTransform[2][0] = glm::clamp(cursorTransform[2][0] + diffx / SCREEN_WIDTH, -1.0f, 1.0f);
		cursorTransform[2][1] = glm::clamp(cursorTransform[2][1] - diffy / SCREEN_HEIGHT, -1.0f, 1.0f);
		if (mouseFlags & MOUSE_LEFTBUTTON_PRESSED && cursorTransform[2][0] < -0.9 && cursorTransform[2][1] < -0.9) {
			playing = !playing;
		}
		if (mouseFlags & MOUSE_RIGHTBUTTON_HOLD) {
			cursorShader.setVector("color", glm::vec4(glm::vec3(1.0f), 0.0f));
		} else {
			cursorShader.setVector("color", glm::vec4(1.0f));
		}
	});
	Model cursor = ModelLoader::loadShpFile("res/shapes/cursor.shp");
	Model play = ModelLoader::loadShpFile("res/shapes/play.shp");
	Model pause = ModelLoader::loadShpFile("res/shapes/pause.shp");

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

		shaderProgram.setMatrix("transform", script->getTranscationMatrixAt(acumulatedTime));

		shaderProgram.use();
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
		cursorShader.setMatrix("transform", cursorTransform);
		cursor.draw(cursorShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}