#include "Application.h"

// TODO (Michael/Yicong/Ilias): Replace this with your boid gen and behavior stuff when it works
#include "BoidGenTemp.h"

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
    // The Main Game Loop
    while (!graphics.ShouldClose())
    {

        // 1. Calculate Time
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // 2. Process OS movement input when unpaused
        if (currentState == AppState::SIMULATION)
        {
            graphics.ProcessInput();
        }

        // 3. Handle Pause Toggle (Escape Key)
        bool escapeIsPressed = (glfwGetKey(graphics.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS);

        if (escapeIsPressed && !escapeWasPressed)
        {
            if (currentState == AppState::SIMULATION)
            {
                // Only allow pausing if the game is actively running
                previousState = currentState;
                currentState = AppState::PAUSED;
            }
            else if (currentState == AppState::PAUSED)
            {
                // Unpause and return to where we were
                currentState = previousState;
            }
            // If the user is in the setup menu, the Escape key is completely ignored.
        }
        escapeWasPressed = escapeIsPressed;

        // 4. Start UI Frame
        uiManager.BeginFrame();

        // 5. Execute Current State
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

        // 6. Draw UI and Swap Buffers
        uiManager.EndFrame();
        graphics.SwapBuffers();
    }
}

// ---------------------------------------------------------
// STATE HANDLERS
// ---------------------------------------------------------

void Application::RunSetupState()
{
    graphics.Render(TEMPORARY_dummyBoidData, false, simulationTime, configMaxAltitude, configEarthRadius);

    bool startClicked = uiManager.RenderSetupMenu(
        configBoidCount,
        configEarthRadius,
        configMinAltitude,
        configMaxAltitude,
        graphics.GetSunOrbitDistance());

    if (startClicked)
    {
        // TODO (Michael/Yicong/Ilias): Replace this with actual boid generation code one that's done
        // TEMPORARY_dummyBoidData = GenerateTestBoids(configBoidCount, configMinAltitude, configMaxAltitude);
        // preyFlock = Flock(configBoidCount); // Uncomment when it works - Kyle

        flock = Flock(configBoidCount, configMinAltitude, configMaxAltitude);
        currentState = AppState::SIMULATION;
    }
}

// Here's a finalised RunSimState func -Ilias // Edited - Kyle
void Application::RunSimulationState(float deltaTime)
{
    simulationTime += deltaTime;

    // preyFlock.update(); // Uncomment when it works - Kyle

    // Uncomment when it works - Kyle

    boidDataToRender = BoidRenderer::BoidsToMatrices(flock.GetFlock());

    graphics.Render(boidDataToRender, true, simulationTime, configMaxAltitude, configEarthRadius);

    flock.Update(deltaTime);

    // graphics.Render(TEMPORARY_dummyBoidData, true, simulationTime, configMaxAltitude, configEarthRadius);
}

void Application::RunPausedState()
{
    // Draw everything frozen in the background
    // graphics.Render(TEMPORARY_dummyBoidData, true, simulationTime, configMaxAltitude, configEarthRadius);
    graphics.Render(boidDataToRender, true, simulationTime, configMaxAltitude, configEarthRadius);

    bool resumeClicked, setupClicked, quitClicked;
    uiManager.RenderPauseMenu(resumeClicked, setupClicked, quitClicked);

    if (resumeClicked)
        currentState = previousState;
    if (setupClicked)
        currentState = AppState::SETUP;
    if (quitClicked)
        glfwSetWindowShouldClose(graphics.GetWindow(), true);
}