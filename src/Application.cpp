#include "Application.h"
#include "BoidGenTemp.h"
#include <GLFW/glfw3.h>

Application::Application()
{
    // Initialize the visual system
    graphics.Initialize(1280, 720, "Boids");

    // Generate  static dummy matrices
    dummyBoidData = GenerateTestBoids(5000, 10.0f, 15.0f);
}

void Application::Run()
{
    while (!graphics.ShouldClose())
    {
        // Process OS window events
        graphics.ProcessInput();

        // (Physics/Simulation goes here later)

        // Hand the computed matrices to the visual system to draw
        graphics.Render(dummyBoidData);
    }
}