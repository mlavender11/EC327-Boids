#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window.h"
#include "Camera.h"
#include "Callbacks.h"
#include "Earth.h"
#include "Shader.h"
#include "BoidRenderer.h"
#include "BoidGenTemp.h"

int main()
{
    // Set up environment
    GLFWwindow *window = InitializeWindow(1280, 720, "Boids rendering test");
    if (!window)
    {
        return -1;
    }

    // Set up callbacks for camera control
    CameraState camState;
    glfwSetWindowUserPointer(window, &camState);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Load assets
    Shader earthShader("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
    Shader boidShader("shaders/BoidVertexShader.glsl", "shaders/FragmentShader.glsl");

    Earth earthModel(10.0f, 16, 16);
    BoidRenderer boidRenderer; // Boids

    // Generate 5000 dummy boids flying between the surface (10.0) and the upper atmosphere (15.0)
    std::vector<glm::mat4> boidData = GenerateTestBoids(5000, 10.0f, 15.0f);

    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        // Calculate matrices
        glm::mat4 view = CalculateViewMatrix(camState);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        // Clear screen
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw Earth
        earthShader.use();
        earthShader.setMat4("view", view);
        earthShader.setMat4("projection", projection);
        earthShader.setMat4("model", glm::mat4(1.0f));
        earthModel.Draw();

        // Draw the 5000 boids
        boidShader.use();
        boidShader.setMat4("view", view);
        boidShader.setMat4("projection", projection);
        boidRenderer.DrawInstanced(boidData);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}