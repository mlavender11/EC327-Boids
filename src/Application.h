#ifndef APPLICATION_H
#define APPLICATION_H

#include "visual/GraphicsEngine.h"
#include "visual/UIManager.h"
#include <vector> // Dont think its really needed cause flock is already a vector
#include <glm/glm.hpp>
#include "Flock.h"

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

    // --- Setup Configuration Variables ---
    int configBoidCount = 500;
    float configEarthRadius = 10.0f;
    float configMinAltitude = 10.0f;
    float configMaxAltitude = 20.0f;

    // --- Live Config Variables ---
    float boidCohesion = 1.0f;
    float boidSeparation = 1.0f;
    float boidAlignment = 1.0f;
    float boidVisualRange = 5.0f;

    Flock flock;

    std::vector<glm::mat4> boidDataToRender;
};

#endif