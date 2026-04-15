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

    float surfaceRadius = 30.0f;
    float atmosphereRadius = 40.0f;

    camState.maxZoom = atmosphereRadius * 3.0f;
    camState.minZoom = surfaceRadius * 1.1f; // Set min zoom just above the surface radius

    Earth earthModel(surfaceRadius, 16, 16);
    BoidRenderer boidRenderer; // Boids

    // Generate 5000 coordinates to simulate boids between surfaceRadius and atmosphereRadius
    std::vector<glm::mat4> boidData = GenerateTestBoids(5000, surfaceRadius, atmosphereRadius);

    while (!glfwWindowShouldClose(window))
    {
        ProcessInput(window);

        // Calculate matrices
        glm::mat4 view = CalculateViewMatrix(camState);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
        glm::mat4 model = glm::mat4(1.0f);

        // RENDERING SECTION
        // Clear screen
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Define our Light (e.g., coming from the top-right-front)
        glm::vec3 sunDirection(-0.2f, -1.0f, -0.3f);
        glm::vec3 sunColor(1.0f, 0.95f, 0.9f); // Slightly warm white
        glm::vec3 ambient(1.0f, 1.0f, 1.0f);   // White ambient base

        // Draw Earth
        earthShader.use();
        earthShader.setMat4("view", view);
        earthShader.setMat4("projection", projection);
        earthShader.setMat4("model", glm::mat4(1.0f));

        // Send lighting uniforms
        glUniform3fv(glGetUniformLocation(earthShader.ID, "lightDir"), 1, glm::value_ptr(sunDirection));
        glUniform3fv(glGetUniformLocation(earthShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
        glUniform3fv(glGetUniformLocation(earthShader.ID, "ambientColor"), 1, glm::value_ptr(ambient));

        earthModel.Draw();

        // Draw Boids
        boidShader.use();
        boidShader.setMat4("view", view);
        boidShader.setMat4("projection", projection);

        // Send lighting uniforms to the boid shader
        glUniform3fv(glGetUniformLocation(boidShader.ID, "lightDir"), 1, glm::value_ptr(sunDirection));
        glUniform3fv(glGetUniformLocation(boidShader.ID, "lightColor"), 1, glm::value_ptr(sunColor));
        glUniform3fv(glGetUniformLocation(boidShader.ID, "ambientColor"), 1, glm::value_ptr(ambient));

        boidRenderer.DrawInstanced(boidData);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}