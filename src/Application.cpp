#include "Application.h"
#include <GLFW/glfw3.h>
#include <cmath>

static glm::vec3 RandomSphericalPoint(float minR, float maxR)
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dr(minR, maxR);
    std::uniform_real_distribution<float> dtheta(0.0f, 2.0f * 3.14159265f);
    std::uniform_real_distribution<float> dcosphi(-1.0f, 1.0f);

    float r = dr(gen);
    float theta = dtheta(gen);
    float cosPhi = dcosphi(gen);
    float sinPhi = std::sqrt(1.0f - cosPhi * cosPhi);
    return glm::vec3(r * sinPhi * std::cos(theta),
                     r * sinPhi * std::sin(theta),
                     r * cosPhi);
}

static glm::mat4 OrientedModel(glm::vec3 position, glm::vec3 velocity, float scale)
{
    glm::vec3 forward = (glm::dot(velocity, velocity) > 1e-6f)
                            ? glm::normalize(velocity)
                            : glm::vec3(0.0f, 0.0f, 1.0f);

    float posLen = glm::length(position);
    glm::vec3 radialUp = (posLen > 1e-6f) ? position / posLen : glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 right = glm::cross(radialUp, forward);
    float rightLen = glm::length(right);
    if (rightLen < 1e-6f)
    {
        right = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), forward);
        rightLen = glm::length(right);
        if (rightLen < 1e-6f)
            right = glm::vec3(1.0f, 0.0f, 0.0f);
        else
            right /= rightLen;
    }
    else
    {
        right /= rightLen;
    }

    glm::vec3 localUp = glm::normalize(glm::cross(forward, right));

    glm::mat4 model(1.0f);
    model[0] = glm::vec4(right, 0.0f);
    model[1] = glm::vec4(localUp, 0.0f);
    model[2] = glm::vec4(forward, 0.0f);
    model[3] = glm::vec4(position, 1.0f);
    return glm::scale(model, glm::vec3(scale));
}

Application::Application()
{
    graphics.Initialize(1280, 720, "Boids");
    uiManager.Initialize(graphics.GetWindow());

    currentState = AppState::SETUP;
    previousState = AppState::SETUP;
    escapeWasPressed = false;
    simulationTime = 0.0f;
    lastFrameTime = glfwGetTime();
}

void Application::Run()
{
    while (!graphics.ShouldClose())
    {
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if (currentState == AppState::SIMULATION)
            graphics.ProcessInput();

        bool escapeIsPressed = (glfwGetKey(graphics.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS);
        if (escapeIsPressed && !escapeWasPressed)
        {
            if (currentState == AppState::SIMULATION)
            {
                previousState = currentState;
                currentState = AppState::PAUSED;
            }
            else if (currentState == AppState::PAUSED || currentState == AppState::GRAPHICS_SETTINGS)
            {
                currentState = (currentState == AppState::PAUSED) ? previousState : AppState::PAUSED;
            }
        }
        escapeWasPressed = escapeIsPressed;

        uiManager.BeginFrame();

        if (currentState == AppState::SETUP)
            RunSetupState();
        else if (currentState == AppState::SIMULATION)
            RunSimulationState(deltaTime);
        else if (currentState == AppState::PAUSED || currentState == AppState::GRAPHICS_SETTINGS)
            RunPausedState();

        uiManager.EndFrame();
        graphics.SwapBuffers();
    }
}

void Application::CreatePredators()
{
    predators.clear();
    for (int i = 0; i < configPredatorCount; i++)
        predators.push_back(Predator(
            RandomSphericalPoint(configMinAltitude, configMaxAltitude),
            configMinAltitude, configMaxAltitude));
}

void Application::BuildPredatorMatrices(std::vector<glm::mat4> &hunting, std::vector<glm::mat4> &full)
{
    hunting.clear();
    full.clear();
    for (Predator &p : predators)
    {
        if (p.IsHunting())
        {
            hunting.push_back(OrientedModel(p.GetPosition(), p.GetVelocity(), 0.45f));
        }
        else
        {
            full.push_back(OrientedModel(p.GetPosition(), p.GetVelocity(), 0.45f));
        }
    }
}

void Application::RunSetupState()
{
    graphics.Render(boidDataToRender, {}, {}, true, simulationTime,
                    configMaxAltitude, configMinAltitude, configEarthRadius);

    simulationTime = 0.0f;
    lastFrameTime = glfwGetTime();

    bool quitClicked = false;
    uiManager.ResetPromptDismissal();

    bool startClicked = uiManager.RenderSetupMenu(
        configBoidCount, configEarthRadius,
        configMinAltitude, configMaxAltitude,
        graphics.GetSunOrbitDistance(), configSunSpeed,
        configPredatorCount, quitClicked);

    if (quitClicked)
        glfwSetWindowShouldClose(graphics.GetWindow(), true);

    if (startClicked)
    {
        graphics.SetSunSpeed(configSunSpeed);
        flock = Flock(configBoidCount, configMinAltitude, configMaxAltitude);
        CreatePredators();
        currentState = AppState::SIMULATION;
    }
}

void Application::RunSimulationState(float deltaTime)
{
    simulationTime += deltaTime;

    bool optionsClicked = false;

    uiManager.RenderSimulationOverlay(boidCohesion, boidSeparation, boidAlignment,
                                      boidVisualRange, boidMaxSpeed, boidMaxForce,
                                      predatorMaxSpeed, predatorMaxForce, predatorHungerRate,
                                      simulationTime, optionsClicked);

    if (optionsClicked)
    {
        previousState = currentState; // Remember we were in SIMULATION
        currentState = AppState::PAUSED;
    }

    flock.Update(deltaTime, predators,
                 boidCohesion, boidSeparation, boidAlignment,
                 boidVisualRange, boidMaxSpeed, boidMaxForce);

    // Update the predators with the live variables before calculating physics
    for (Predator &p : predators)
    {
        p.SetMaxSpeed(predatorMaxSpeed);
        p.SetMaxForce(predatorMaxForce);
        p.SetHungerRate(predatorHungerRate);
        p.Update(deltaTime, flock.GetFriendlies());
    }

    // Boids (alive only) in gold, predators in red — separate draw calls
    boidDataToRender = BoidRenderer::BoidsToMatricesAlive(flock.GetFlock());

    std::vector<glm::mat4> huntingPredators;
    std::vector<glm::mat4> fullPredators;
    BuildPredatorMatrices(huntingPredators, fullPredators);

    graphics.Render(boidDataToRender, huntingPredators, fullPredators, true, simulationTime,
                    configMaxAltitude, configMinAltitude, configEarthRadius);
}

void Application::RunPausedState()
{
    graphics.Render(boidDataToRender, {}, {}, true, simulationTime,
                    configMaxAltitude, configMinAltitude, configEarthRadius);

    if (currentState == AppState::PAUSED)
    {
        bool resumeClicked, setupClicked, quitClicked, graphicsClicked;
        uiManager.RenderPauseMenu(resumeClicked, setupClicked, quitClicked, graphicsClicked);

        if (resumeClicked)
            currentState = previousState;
        if (setupClicked)
            currentState = AppState::SETUP;
        if (graphicsClicked)
            currentState = AppState::GRAPHICS_SETTINGS;
        if (quitClicked)
            glfwSetWindowShouldClose(graphics.GetWindow(), true);
    }
    else if (currentState == AppState::GRAPHICS_SETTINGS)
    {
        bool backToPause = false;
        UITheme currentUITheme = uiManager.GetActiveTheme();
        uiManager.RenderGraphicsMenu(currentUITheme, backToPause);
        graphics.ToggleColdWar(uiManager.GetActiveTheme() == UITheme::COLDWAR);
        if (backToPause)
            currentState = AppState::PAUSED;
    }
}