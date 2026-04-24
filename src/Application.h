#ifndef APPLICATION_H
#define APPLICATION_H

#include "visual/GraphicsEngine.h"
#include "visual/UIManager.h"

#include "Flock.h"
#include "Predator.h"

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
    void RunSetupState();
    void RunSimulationState(float deltaTime);
    void RunPausedState();

    void CreatePredators();
    std::vector<glm::mat4> BuildRenderMatrices();

    GraphicsEngine graphics;
    UIManager uiManager;

    AppState currentState;
    AppState previousState;
    bool escapeWasPressed;

    float simulationTime;
    float lastFrameTime;

    int configBoidCount = 500;
    int configPredatorCount = 5;

    float configEarthRadius = 10.0f;
    float configMinAltitude = 10.0f;
    float configMaxAltitude = 20.0f;
    float configSunSpeed = 0.3f;

    float boidCohesion = 1.0f;
    float boidSeparation = 1.0f;
    float boidAlignment = 1.0f;
    float boidVisualRange = 5.0f;
    float boidMaxSpeed = 5.0f;
    float boidMaxForce = 10.0f;

    Flock flock;
    std::vector<Predator> predators;

    std::vector<glm::mat4> boidDataToRender;
};

#endif