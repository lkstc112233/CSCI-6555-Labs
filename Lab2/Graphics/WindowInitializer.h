#ifndef GRAPHICS_WINDOWINITIALIZER_H
#define GRAPHICS_WINDOWINITIALIZER_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// A function that manages the messy operations that initializes the window.
GLFWwindow* initializeWindow(const char* title, int width = SCREEN_WIDTH,
                             int height = SCREEN_HEIGHT);

#endif  // GRAPHICS_WINDOWINITIALIZER_H
