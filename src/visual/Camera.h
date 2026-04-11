#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Callbacks.h" // Needed to access CameraState struct

// Calculates the View Matrix based on the current spherical coordinates
glm::mat4 CalculateViewMatrix(const CameraState &cam);

#endif