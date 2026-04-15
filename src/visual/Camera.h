#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

// Calculates the View Matrix based on the current spherical coordinates
glm::mat4 CalculateViewMatrix(const CameraState &cam);

#endif