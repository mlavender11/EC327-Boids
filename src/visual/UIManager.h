#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <GLFW/glfw3.h>

class UIManager
{
public:
    UIManager();
    ~UIManager();

    // Set up the ImGui context using the engine's window
    void Initialize(GLFWwindow *);

    void BeginFrame();
    void EndFrame();

    // Draws the setup menu. Returns TRUE if the user clicked "Start"
    bool RenderSetupMenu(int &boidCount, float &earthRadius, float &minAlt, float &maxAlt);

    // Draws the pause menu. Returns TRUE if the user clicked "Resume"
    void RenderPauseMenu(bool &resume, bool &setup, bool &quit);
};

#endif