#include "Application.h"
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
    while (!graphics.ShouldClose())
    {

        // --- 1. CLOCK MATH ---
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // ONLY advance the game clock if we are actively simulating!
        if (currentState == AppState::SIMULATION)
        {
            simulationTime += deltaTime;
        }

        graphics.ProcessInput();

        // --- 2. ESCAPE KEY TOGGLE LOGIC ---
        bool escapeIsPressed = (glfwGetKey(graphics.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS);

        // Only trigger exactly once when the key is first pressed down
        if (escapeIsPressed && !escapeWasPressed)
        {
            if (currentState == AppState::PAUSED)
            {
                // Unpause and go back to what we were doing
                currentState = previousState;
            }
            else
            {
                // Pause the game and remember where we came from
                previousState = currentState;
                currentState = AppState::PAUSED;
            }
        }
        escapeWasPressed = escapeIsPressed; // Update the tracking variable

        // --- 3. RENDER CYCLE ---
        uiManager.BeginFrame();

        if (currentState == AppState::SETUP)
        {
            // Pass simulationTime to Render
            graphics.Render(dummyBoidData, false, simulationTime);

            if (uiManager.RenderSetupMenu(configBoidCount, configEarthRadius, configMinAltitude, configMaxAltitude))
            {
                dummyBoidData = GenerateTestBoids(configBoidCount, configMinAltitude, configMaxAltitude);
                currentState = AppState::SIMULATION;
            }
        }
        else if (currentState == AppState::SIMULATION)
        {
            // Pass simulationTime to Render
            graphics.Render(dummyBoidData, true, simulationTime);
        }
        else if (currentState == AppState::PAUSED)
        {
            // Pass simulationTime to Render
            graphics.Render(dummyBoidData, true, simulationTime);

            bool resumeClicked, setupClicked, quitClicked;
            uiManager.RenderPauseMenu(resumeClicked, setupClicked, quitClicked);

            if (resumeClicked)
                currentState = previousState;
            if (setupClicked)
                currentState = AppState::SETUP;
            if (quitClicked)
                glfwSetWindowShouldClose(graphics.GetWindow(), true);
        }

        uiManager.EndFrame();
        graphics.SwapBuffers();
    }
}