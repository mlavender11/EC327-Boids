#include "Application.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <random>

// Helper: generate a uniformly random point on the spherical shell [minR, maxR].
static glm::vec3 RandomSphericalPoint(float minR, float maxR)
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> distrib_r(minR, maxR);
    std::uniform_real_distribution<float> distrib_theta(0.0f, 2.0f * (float)M_PI);
    std::uniform_real_distribution<float> distrib_cos_phi(-1.0f, 1.0f);

    float r        = distrib_r(gen);
    float theta    = distrib_theta(gen);
    float cos_phi  = distrib_cos_phi(gen);
    float sin_phi  = std::sqrt(1.0f - cos_phi * cos_phi);

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
        {
            graphics.ProcessInput();
        }

        bool escapeIsPressed =
            glfwGetKey(graphics.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS;

        if (escapeIsPressed && !escapeWasPressed)
        {
            if (currentState == AppState::SIMULATION)
            {
                previousState = currentState;
                currentState = AppState::PAUSED;
            }
            else if (currentState == AppState::PAUSED)
            {
                currentState = previousState;
            }
        }

        escapeWasPressed = escapeIsPressed;

        uiManager.BeginFrame();

        if (currentState == AppState::SETUP)
        {
            RunSetupState();
        }
        else if (currentState == AppState::SIMULATION)
        {
            RunSimulationState(deltaTime);
        }
        else if (currentState == AppState::PAUSED)
        {
            RunPausedState();
        }

        uiManager.EndFrame();
        graphics.SwapBuffers();
    }
}

void Application::CreatePredators()
{
    predators.clear();

    // FIX: spawn predators on the configured spherical shell, not a hardcoded cube.
    for (int i = 0; i < configPredatorCount; i++)
    {
        predators.push_back(
            Predator(RandomSphericalPoint(configMinAltitude, configMaxAltitude),
                     configMinAltitude, configMaxAltitude)
        );
    }
}

// Build a velocity-oriented model matrix for any moving entity.
// The mesh's forward axis (+Z) is aligned to the entity's velocity direction,
// and the local "up" axis is biased toward the radial (away-from-earth) direction.
// This mirrors BoidRenderer::BoidsToMatrices() from the original codebase.
static glm::mat4 OrientedModel(glm::vec3 position, glm::vec3 velocity, float scale)
{
    // Fall back to +Z if the entity is stationary to avoid normalize(0).
    glm::vec3 forward = (glm::dot(velocity, velocity) > 1e-6f)
                            ? glm::normalize(velocity)
                            : glm::vec3(0.0f, 0.0f, 1.0f);

    // Radial direction (away from earth centre) as the reference up.
    float posLen = glm::length(position);
    glm::vec3 radialUp = (posLen > 1e-6f)
                             ? position / posLen
                             : glm::vec3(0.0f, 1.0f, 0.0f);

    // If forward is parallel to radialUp, pick an arbitrary fallback right.
    glm::vec3 right = glm::cross(radialUp, forward);
    float rightLen = glm::length(right);
    if (rightLen < 1e-6f) {
        right = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), forward);
        rightLen = glm::length(right);
        if (rightLen < 1e-6f)
            right = glm::vec3(1.0f, 0.0f, 0.0f);
        else
            right /= rightLen;
    } else {
        right /= rightLen;
    }

    glm::vec3 localUp = glm::normalize(glm::cross(forward, right));

    // Assemble rotation + translation directly (column-major).
    glm::mat4 model(1.0f);
    model[0] = glm::vec4(right,    0.0f);
    model[1] = glm::vec4(localUp,  0.0f);
    model[2] = glm::vec4(forward,  0.0f);
    model[3] = glm::vec4(position, 1.0f);

    return glm::scale(model, glm::vec3(scale));
}

std::vector<glm::mat4> Application::BuildRenderMatrices()
{
    std::vector<glm::mat4> matrices;

    for (Friendly& friendly : flock.GetFriendlies())
    {
        float scale = friendly.IsAlive() ? 0.25f : 0.08f;
        matrices.push_back(
            OrientedModel(friendly.GetPosition(), friendly.GetVelocity(), scale)
        );
    }

    for (Predator& predator : predators)
    {
        matrices.push_back(
            OrientedModel(predator.GetPosition(), predator.GetVelocity(), 0.45f)
        );
    }

    return matrices;
}

void Application::RunSetupState()
{
    boidDataToRender = BuildRenderMatrices();

    graphics.Render(
        boidDataToRender,
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

        // FIX: spawn friendlies on the configured spherical shell instead of a
        // hardcoded [-15,15]^3 cube, so they respect the earth radius and
        // altitude settings chosen on the setup screen.
        for (int i = 0; i < configBoidCount; i++)
        {
            flock.AddFriendly(
                Friendly(RandomSphericalPoint(configMinAltitude, configMaxAltitude),
                         configMinAltitude, configMaxAltitude)
            );
        }

        CreatePredators();

        simulationTime = 0.0f;
        lastFrameTime = glfwGetTime();

        currentState = AppState::SIMULATION;
    }
}

void Application::RunSimulationState(float deltaTime)
{
    simulationTime += deltaTime;

    // FIX: RenderSimulationOverlay updates the slider variables in-place, so
    // call it before Update so the new values are used this frame.
    uiManager.RenderSimulationOverlay(
        boidCohesion,
        boidSeparation,
        boidAlignment,
        boidVisualRange,
        boidMaxSpeed,
        boidMaxForce
    );

    // FIX: pass the live UI slider values into flock.Update so cohesion,
    // separation, alignment, visual range, speed, and force actually take effect.
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
    {
        predator.Update(deltaTime, flock.GetFriendlies());
    }

    boidDataToRender = BuildRenderMatrices();

    graphics.Render(
        boidDataToRender,
        true,
        simulationTime,
        configMaxAltitude,
        configEarthRadius
    );
}

void Application::RunPausedState()
{
    graphics.Render(
        boidDataToRender,
        true,
        simulationTime,
        configMaxAltitude,
        configEarthRadius
    );

    bool resumeClicked;
    bool setupClicked;
    bool quitClicked;

    uiManager.RenderPauseMenu(
        resumeClicked,
        setupClicked,
        quitClicked
    );

    if (resumeClicked)
    {
        currentState = previousState;
    }

    if (setupClicked)
    {
        currentState = AppState::SETUP;
    }

    if (quitClicked)
    {
        glfwSetWindowShouldClose(graphics.GetWindow(), true);
    }
}