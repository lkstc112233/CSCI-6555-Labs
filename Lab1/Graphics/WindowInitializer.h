#ifndef GRAPHICS_WINDOWINITIALIZER_H
#define GRAPHICS_WINDOWINITIALIZER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

GLFWwindow *initializeWindow(const char* title, int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT);

#endif // GRAPHICS_WINDOWINITIALIZER_H
