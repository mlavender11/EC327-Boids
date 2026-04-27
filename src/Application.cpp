#include "Application.h"

#include <GLFW/glfw3.h>

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

        // Handle Escape Logic
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

        // State handling
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

// ---------------------------------------------------------
// STATE HANDLERS
// ---------------------------------------------------------

void Application::RunSetupState()
{
    boidDataToRender = BoidRenderer::BoidsToMatrices(flock.GetFlock());
    graphics.Render(boidDataToRender, true, simulationTime, configMaxAltitude, configMinAltitude, configEarthRadius);

    // Reset simulation time
    simulationTime = 0.0f;
    lastFrameTime = glfwGetTime();

    bool quitClicked = false;
    uiManager.ResetPromptDismissal(); // Reset prompt dismissal for new simulation

    bool startClicked = uiManager.RenderSetupMenu(
        configBoidCount,
        configEarthRadius,
        configMinAltitude,
        configMaxAltitude,
        graphics.GetSunOrbitDistance(),
        configSunSpeed,
        quitClicked);

    if (quitClicked)
    {
        glfwSetWindowShouldClose(graphics.GetWindow(), true);
    }

    if (startClicked)
    {
        graphics.SetSunSpeed(configSunSpeed);
        flock = Flock(configBoidCount, configMinAltitude, configMaxAltitude);
        currentState = AppState::SIMULATION;
    }
}

void Application::RunSimulationState(float deltaTime)
{
    simulationTime += deltaTime;

    // Live update of boid parameters from the UI sliders first
    uiManager.RenderSimulationOverlay(boidCohesion, boidSeparation, boidAlignment, boidVisualRange, boidMaxSpeed, boidMaxForce, simulationTime);
    flock.Update(deltaTime, boidCohesion, boidSeparation, boidAlignment, boidVisualRange, boidMaxSpeed, boidMaxForce);

    // Render the boids using Graphics Engine
    boidDataToRender = BoidRenderer::BoidsToMatrices(flock.GetFlock());
    graphics.Render(boidDataToRender, true, simulationTime, configMaxAltitude, configMinAltitude, configEarthRadius);
}

void Application::RunPausedState()
{
    // Always render the background so the screen isn't black
    graphics.Render(boidDataToRender, true, simulationTime, configMaxAltitude, configMinAltitude, configEarthRadius);

    if (currentState == AppState::PAUSED)
    {
        bool resumeClicked, setupClicked, quitClicked, graphicsClicked;
        uiManager.RenderPauseMenu(resumeClicked, setupClicked, quitClicked, graphicsClicked);

        if (resumeClicked)
            currentState = previousState;
        if (setupClicked)
            currentState = AppState::SETUP;
        if (graphicsClicked)
            currentState = AppState::GRAPHICS_SETTINGS; // Switches to sub-menu
        if (quitClicked)
            glfwSetWindowShouldClose(graphics.GetWindow(), true);
    }
    else if (currentState == AppState::GRAPHICS_SETTINGS)
    {
        bool backToPause = false;
        UITheme currentUITheme = uiManager.GetActiveTheme();
        uiManager.RenderGraphicsMenu(currentUITheme, backToPause);

        if (backToPause)
            currentState = AppState::PAUSED;
    }
}