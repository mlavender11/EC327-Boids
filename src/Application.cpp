#include "Application.h"
#include "BoidGenTemp.h"
#include <GLFW/glfw3.h>

Application::Application()
{
    graphics.Initialize(1280, 720, "Boids");

    // Initialize the UI using the window created by the Graphics Engine
    uiManager.Initialize(graphics.GetWindow());

    currentState = AppState::SETUP;
}

void Application::Run()
{
    while (!graphics.ShouldClose())
    {
        graphics.ProcessInput();

        // Tell ImGui a new frame is starting
        uiManager.BeginFrame();

        // Handle State Logic & Rendering
        if (currentState == AppState::SETUP)
        {
            graphics.Render(dummyBoidData, false);

            // RenderSetupMenu returns true if the Start button was clicked
            if (uiManager.RenderSetupMenu(configBoidCount, configEarthRadius, configMinAltitude, configMaxAltitude))
            {
                dummyBoidData = GenerateTestBoids(configBoidCount, configMinAltitude, configMaxAltitude);
                currentState = AppState::SIMULATION;
            }
        }
        else if (currentState == AppState::SIMULATION)
        {
            // Behavior simulation goes here later, for now just render the boids with dummy data

            graphics.Render(dummyBoidData, true);
        }

        // Draw the UI on top of the 3D scene
        uiManager.EndFrame();

        // Finally, swap the buffers to display the combined frame to the monitor
        graphics.SwapBuffers();
    }
}