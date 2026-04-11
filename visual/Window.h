#ifndef WINDOWS_H
#define WINDOWS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Initializes GLFW, GLAD, and creates the window
GLFWwindow *InitializeWindow(int width, int height, const char *title);

// Handles continuous polling of keyboard input
void ProcessInput(GLFWwindow *window);

#endif