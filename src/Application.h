#ifndef APPLICATION_H
#define APPLICATION_H

#include "visual/GraphicsEngine.h"
#include "visual/UIManager.h"
#include <vector>
#include <glm/glm.hpp>
#include "Flock.h"
#include "behavior/Predator.h"

enum class AppState
{
    SETUP,
    SIMULATION,
    PAUSED,
    GRAPHICS_SETTINGS
};

class Application
{
public:
    Application();
    void Run();

private:
    void RunSetupState();
    void RunSimulationState(float deltaTime);
    void RunPausedState();

    void CreatePredators();
    void BuildPredatorMatrices(std::vector<glm::mat4> &hunting, std::vector<glm::mat4> &full);

    GraphicsEngine graphics;
    UIManager uiManager;

    AppState currentState;
    AppState previousState;
    bool escapeWasPressed;

    float simulationTime;
    float lastFrameTime;

    // --- Setup Config Variables ---
    int configBoidCount = 500;
    int configPredatorCount = 3;
    float configEarthRadius = 5.0f;
    float configMinAltitude = 5.0f;
    float configMaxAltitude = 10.0f;
    float configSunSpeed = 0.3f;

    // --- Live Boid Config Variables ---
    float boidCohesion = 1.0f;
    float boidSeparation = 1.0f;
    float boidAlignment = 1.0f;
    float boidVisualRange = 5.0f;
    float boidMaxSpeed = 5.0f;
    float boidMaxForce = 10.0f;

    // --- Live Predator Config Variables ---
    float predatorMaxSpeed = 4.0f;
    float predatorMaxForce = 0.6f;
    float predatorHungerRate = 0.04f;

    Flock flock;
    std::vector<Predator> predators;
    std::vector<glm::mat4> boidDataToRender;
};

#endif