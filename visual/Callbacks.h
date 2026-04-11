#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <GLFW/glfw3.h>

// https://www.forrestthewoods.com/blog/global-variables-are-evil-and-unsafe
// Apparently global variables shouldn't be used, so will use a struct to store "global" variables

struct CameraState
{
    float radius = 15.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;

    bool isDragging = false; // Track if the mouse is being dragged
    float lastX = 640.0f;    // Default is middle of the window
    float lastY = 360.0f;    // Default is middle of the window
};

// Callback declarations
void mouse_button_callback(GLFWwindow *, int, int, int);
void cursor_position_callback(GLFWwindow *, double, double);
void scroll_callback(GLFWwindow *, double, double);

#endif