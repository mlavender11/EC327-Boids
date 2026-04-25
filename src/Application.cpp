#include "Application.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <random>

static glm::vec3 RandomSphericalPoint(float minR, float maxR)
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> distrib_r(minR, maxR);
    std::uniform_real_distribution<float> distrib_theta(0.0f, 2.0f * (float)M_PI);
    std::uniform_real_distribution<float> distrib_cos_phi(-1.0f, 1.0f);

    float r       = distrib_r(gen);
    float theta   = distrib_theta(gen);
    float cos_phi = distrib_cos_phi(gen);
    float sin_phi = std::sqrt(1.0f - cos_phi * cos_phi);

    return glm::vec3(
        r * sin_phi * std::cos(theta),
        r * sin_phi * std::sin(theta),
        r * cos_phi
    );
}

Application::Application()
{
    graphics.Initialize(1280, 720, "Boids");
    uiManager.Initialize(graphics.GetWindow());

    currentState     = AppState::SETUP;
    previousState    = AppState::SETUP;
    escapeWasPressed = false;
    simulationTime   = 0.0f;
    lastFrameTime    = glfwGetTime();
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

        bool escapeIsPressed =
            glfwGetKey(graphics.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS;

        if (escapeIsPressed && !escapeWasPressed)
        {
            if (currentState == AppState::SIMULATION)
            {
                previousState = currentState;
                currentState  = AppState::PAUSED;
            }
            else if (currentState == AppState::PAUSED)
            {
                currentState = previousState;
            }
        }

        escapeWasPressed = escapeIsPressed;

        uiManager.BeginFrame();

        if (currentState == AppState::SETUP)
            RunSetupState();
        else if (currentState == AppState::SIMULATION)
            RunSimulationState(deltaTime);
        else if (currentState == AppState::PAUSED)
            RunPausedState();

        uiManager.EndFrame();
        graphics.SwapBuffers();
    }
}

void Application::CreatePredators()
{
    predators.clear();
    for (int i = 0; i < configPredatorCount; i++)
    {
        predators.push_back(
            Predator(RandomSphericalPoint(configMinAltitude, configMaxAltitude),
                     configMinAltitude, configMaxAltitude)
        );
    }
}

static glm::mat4 OrientedModel(glm::vec3 position, glm::vec3 velocity, float scale)
{
    glm::vec3 forward = (glm::dot(velocity, velocity) > 1e-6f)
                            ? glm::normalize(velocity)
                            : glm::vec3(0.0f, 0.0f, 1.0f);

    float posLen = glm::length(position);
    glm::vec3 radialUp = (posLen > 1e-6f)
                             ? position / posLen
                             : glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 right = glm::cross(radialUp, forward);
    float rightLen  = glm::length(right);
    if (rightLen < 1e-6f) {
        right    = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), forward);
        rightLen = glm::length(right);
        if (rightLen < 1e-6f)
            right = glm::vec3(1.0f, 0.0f, 0.0f);
        else
            right /= rightLen;
    } else {
        right /= rightLen;
    }

    glm::vec3 localUp = glm::normalize(glm::cross(forward, right));

    glm::mat4 model(1.0f);
    model[0] = glm::vec4(right,    0.0f);
    model[1] = glm::vec4(localUp,  0.0f);
    model[2] = glm::vec4(forward,  0.0f);
    model[3] = glm::vec4(position, 1.0f);

    return glm::scale(model, glm::vec3(scale));
}

std::vector<glm::mat4> Application::BuildFriendlyMatrices()
{
    std::vector<glm::mat4> matrices;
    for (Friendly& f : flock.GetFriendlies())
    {
        float scale = f.IsAlive() ? 0.25f : 0.08f;
        matrices.push_back(OrientedModel(f.GetPosition(), f.GetVelocity(), scale));
    }
    return matrices;
}

std::vector<glm::mat4> Application::BuildPredatorMatrices()
{
    std::vector<glm::mat4> matrices;
    for (Predator& p : predators)
        matrices.push_back(OrientedModel(p.GetPosition(), p.GetVelocity(), 0.45f));
    return matrices;
}

void Application::RunSetupState()
{
    graphics.Render(
        BuildFriendlyMatrices(),
        BuildPredatorMatrices(),
        true,
        simulationTime,
        configMaxAltitude,
        configEarthRadius
    );

    bool startClicked = uiManager.RenderSetupMenu(
        configBoidCount,
        configEarthRadius,
        configMinAltitude,
        configMaxAltitude,
        graphics.GetSunOrbitDistance(),
        configSunSpeed
    );

    if (startClicked)
    {
        graphics.SetSunSpeed(configSunSpeed);

        flock = Flock();
        for (int i = 0; i < configBoidCount; i++)
        {
            flock.AddFriendly(
                Friendly(RandomSphericalPoint(configMinAltitude, configMaxAltitude),
                         configMinAltitude, configMaxAltitude)
            );
        }

        CreatePredators();

        simulationTime = 0.0f;
        lastFrameTime  = glfwGetTime();

        currentState = AppState::SIMULATION;
    }
}

void Application::RunSimulationState(float deltaTime)
{
    simulationTime += deltaTime;

    uiManager.RenderSimulationOverlay(
        boidCohesion,
        boidSeparation,
        boidAlignment,
        boidVisualRange,
        boidMaxSpeed,
        boidMaxForce
    );

    flock.Update(
        deltaTime,
        predators,
        boidCohesion,
        boidSeparation,
        boidAlignment,
        boidVisualRange,
        boidMaxSpeed,
        boidMaxForce
    );

    for (Predator& predator : predators)
        predator.Update(deltaTime, flock.GetFriendlies());

    graphics.Render(
        BuildFriendlyMatrices(),
        BuildPredatorMatrices(),
        true,
        simulationTime,
        configMaxAltitude,
        configEarthRadius
    );
}

void Application::RunPausedState()
{
    graphics.Render(
        BuildFriendlyMatrices(),
        BuildPredatorMatrices(),
        true,
        simulationTime,
        configMaxAltitude,
        configEarthRadius
    );

    bool resumeClicked;
    bool setupClicked;
    bool quitClicked;

    uiManager.RenderPauseMenu(resumeClicked, setupClicked, quitClicked);

    if (resumeClicked)
        currentState = previousState;

    if (setupClicked)
        currentState = AppState::SETUP;

    if (quitClicked)
        glfwSetWindowShouldClose(graphics.GetWindow(), true);
}