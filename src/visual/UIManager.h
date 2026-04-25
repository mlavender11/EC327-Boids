#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <GLFW/glfw3.h>

class UIManager
{
public:
    UIManager();
    ~UIManager();

    void Initialize(GLFWwindow *);

    void BeginFrame();
    void EndFrame();

    bool RenderSetupMenu(int &boidCount, float &earthRadius, float &minAlt, float &maxAlt, float sunOrbitDistance, float &sunSpeed);

    void RenderSimulationOverlay(float &cohesion, float &separation, float &alignment,
                                  float &visualRange, float &maxSpeed, float &maxForce);

    void RenderPauseMenu(bool &resume, bool &setup, bool &quit);
};

#endif