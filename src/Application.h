#ifndef APPLICATION_H
#define APPLICATION_H

#include "visual/GraphicsEngine.h"
#include "visual/UIManager.h"
#include <vector>
#include <glm/glm.hpp>

enum class AppState
{
    SETUP,
    SIMULATION,
    PAUSED
};

class Application
{
public:
    Application();
    void Run();

private:
    // --- State Handlers ---
    void RunSetupState();
    void RunSimulationState(float deltaTime);
    void RunPausedState();

    // --- Core Engine Variables ---
    GraphicsEngine graphics;
    UIManager uiManager;

    AppState currentState;
    AppState previousState;
    bool escapeWasPressed;

    float simulationTime;
    float lastFrameTime;

    // --- Configuration Variables ---
    int configBoidCount = 5000;
    float configEarthRadius = 10.0f;
    float configMinAltitude = 10.0f;
    float configMaxAltitude = 15.0f;

    // ==========================================
    // TODO: TEMPORARY VARIABLES TO BE REPLACED
    // ==========================================
    // This holds the static boids until the behavior simulation class is built.
    std::vector<glm::mat4> TEMPORARY_dummyBoidData;
};

#endif