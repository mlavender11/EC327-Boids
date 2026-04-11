#include "Camera.h"

glm::mat4 CalculateViewMatrix(const CameraState &cam)
{
    glm::vec3 cameraPos;
    cameraPos.x = cam.radius * cos(glm::radians(cam.pitch)) * sin(glm::radians(cam.yaw));
    cameraPos.y = cam.radius * sin(glm::radians(cam.pitch));
    cameraPos.z = cam.radius * cos(glm::radians(cam.pitch)) * cos(glm::radians(cam.yaw));

    glm::vec3 target(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    return glm::lookAt(cameraPos, target, up);
}