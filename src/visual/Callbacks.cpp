#include "Callbacks.h"

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    // 1. Retrieve the CameraState pointer from the window
    CameraState *cam = static_cast<CameraState *>(glfwGetWindowUserPointer(window));
    if (!cam)
        return; // Safety check

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            cam->isDragging = true;
        }
        else if (action == GLFW_RELEASE)
        {
            cam->isDragging = false;
        }
    }
}

void cursor_position_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    CameraState *cam = static_cast<CameraState *>(glfwGetWindowUserPointer(window));
    if (!cam)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (!cam->isDragging)
    {
        cam->lastX = xpos;
        cam->lastY = ypos;
        return;
    }

    float xoffset = xpos - cam->lastX;
    float yoffset = cam->lastY - ypos;

    cam->lastX = xpos;
    cam->lastY = ypos;

    float sensitivity = 0.3f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cam->yaw -= xoffset;
    cam->pitch += yoffset;

    // Constrain the pitch
    if (cam->pitch > 89.0f)
        cam->pitch = 89.0f;
    if (cam->pitch < -89.0f)
        cam->pitch = -89.0f;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    CameraState *cam = static_cast<CameraState *>(glfwGetWindowUserPointer(window));
    if (!cam)
        return;

    cam->radius -= static_cast<float>(yoffset);

    // Constrain the zoom
    if (cam->radius < 2.0f)
        cam->radius = 2.0f;
    if (cam->radius > 100.0f)
        cam->radius = 100.0f;
}