#include "Callbacks.h"
#include <imgui.h>

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    CameraState *cam = static_cast<CameraState *>(glfwGetWindowUserPointer(window));
    if (!cam)
    {
        return;
    }

    // 1. INTERCEPT CLICKS: If ImGui wants the mouse, ignore the click
    if (ImGui::GetIO().WantCaptureMouse)
        return;

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
    {
        return;
    }

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // Don't intercept ImGui here; just want lastX and lastY to constantly
    // update so the camera doesn't jump when the user clicks off the UI.
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

    // 2. INTERCEPT SCROLLING: If hovering over the UI, don't zoom the camera
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    cam->radius -= static_cast<float>(yoffset);

    if (cam->radius < cam->minZoom)
        cam->radius = cam->minZoom;
    if (cam->radius > cam->maxZoom)
        cam->radius = cam->maxZoom;
}