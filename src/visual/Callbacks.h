#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GLFW/glfw3.h>
#include "Camera.h" // Needed to access CameraState struct

// Callback declarations
void mouse_button_callback(GLFWwindow *, int, int, int);
void cursor_position_callback(GLFWwindow *, double, double);
void scroll_callback(GLFWwindow *, double, double);

#endif