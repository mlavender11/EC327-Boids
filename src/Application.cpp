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
            if (currentState == AppState::PAUSED)
            {
                currentState = previousState; // Unpause
            }
            else
            {
                previousState = currentState; // Pause
                currentState = AppState::PAUSED;
            }
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
    // Draw the 3D world frozen in the background (false = no animation)
    graphics.Render(TEMPORARY_dummyBoidData, false, simulationTime, configMaxAltitude, configEarthRadius);

    // Draw the Setup Menu and wait for the user to click "Start"
    bool startClicked = uiManager.RenderSetupMenu(configBoidCount, configEarthRadius, configMinAltitude, configMaxAltitude);

    if (startClicked)
    {
        // TODO (Michael/Yicong/Ilias): Replace this with actual boid generation code one that's done
        TEMPORARY_dummyBoidData = GenerateTestBoids(configBoidCount, configMinAltitude, configMaxAltitude);

        currentState = AppState::SIMULATION;
    }
}

void Application::RunSimulationState(float deltaTime)
{
    // Advance the game clock
    simulationTime += deltaTime;

    // =========================================================================
    // TODO: time update for behavior simulations
    // =========================================================================
    // 1. Update the boid physics using 'deltaTime'
    //    [behavior something].Update(deltaTime);
    //
    // 2. Grab the newly computed positions/rotations for the graphics engine
    //    std::vector<glm::mat4> boidDataToRender = [behavior something].GetBoidMatrices();
    // =========================================================================

    // TODO (Michael/Yicong/Ilias): Swap out TEMPORARY_dummyBoidData for boidDataToRender
    // or whatever variable holds the real boid data once the simulation is ready
    graphics.Render(TEMPORARY_dummyBoidData, true, simulationTime, configMaxAltitude, configEarthRadius);
}

void Application::RunPausedState()
{
    // Draw everything frozen in the background
    graphics.Render(TEMPORARY_dummyBoidData, true, simulationTime, configMaxAltitude, configEarthRadius);

    bool resumeClicked, setupClicked, quitClicked;
    uiManager.RenderPauseMenu(resumeClicked, setupClicked, quitClicked);

    if (resumeClicked)
        currentState = previousState;
    if (setupClicked)
        currentState = AppState::SETUP;
    if (quitClicked)
        glfwSetWindowShouldClose(graphics.GetWindow(), true);
}