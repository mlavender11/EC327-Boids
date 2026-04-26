#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <GLFW/glfw3.h>
#include "UIThemes.h"

enum class UITheme
{
    MODERN,
    RETRO,
    SYNTHWAVE,
    CLASSIC
};

class UIManager
{
private:
    UITheme activeTheme = UITheme::MODERN; // Default theme is Modern

    ImFont *modernFont;
    ImFont *retroFont;
    ImFont *synthFont;

    bool fontPushedThisFrame = false;

public:
    UIManager();
    ~UIManager();

    // Set up the ImGui context using the engine's window
    void Initialize(GLFWwindow *);

    void BeginFrame();
    void EndFrame();

    // Draws the setup menu. Returns TRUE if the user clicked "Start"
    bool RenderSetupMenu(int &boidCount, float &earthRadius, float &minAlt, float &maxAlt, float sunOrbitDistance, float &sunSpeed);

    // Draws the pause menu. Returns TRUE if the user clicked "Resume"
    void RenderPauseMenu(bool &resume, bool &setup, bool &quit, bool &graphics);

    // Draws the simulation overlay for boid parameters. Returns TRUE if the user changed any of the parameters.
    void RenderSimulationOverlay(float &cohesion, float &separation, float &alignment, float &visualRange, float &maxSpeed, float &maxForce);

    // Draws the graphics settings menu. Returns TRUE if the user clicked "Back"
    void RenderGraphicsMenu(UITheme &currentTheme, bool &backClicked);
    void ApplyTheme(UITheme theme);
    UITheme GetActiveTheme() const;
};

#endif