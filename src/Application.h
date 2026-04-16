#ifndef APPLICATION_H
#define APPLICATION_H

#include "visual/GraphicsEngine.h"
#include "visual/UIManager.h"
#include <vector>
#include <glm/glm.hpp>

enum class AppState
{
    SETUP,
    SIMULATION
};

class Application
{
public:
    Application();
    void Run();

private:
    GraphicsEngine graphics;
    UIManager uiManager;

    std::vector<glm::mat4> dummyBoidData;
    AppState currentState;

    int configBoidCount = 5000;
    float configEarthRadius = 10.0f;
    float configMinAltitude = 10.0f;
    float configMaxAltitude = 15.0f;
};

#endif