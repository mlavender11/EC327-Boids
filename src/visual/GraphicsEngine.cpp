#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine() : window(nullptr) {}

GraphicsEngine::~GraphicsEngine()
{
    delete mainShader;
    delete sunShader;
    delete boidShader;
    delete earth;
    delete sun;
    delete boidRenderer;
    glfwTerminate();
}

bool GraphicsEngine::Initialize(int width, int height, const char *title)
{
    window = InitializeWindow(width, height, title);
    if (!window)
    {
        return false;
    }

    glfwSetWindowUserPointer(window, &camera);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    mainShader = new Shader("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
    atmosphereShader = new Shader("shaders/AtmosphereVertexShader.glsl", "shaders/AtmosphereFragmentShader.glsl");
    sunShader = new Shader("shaders/VertexShader.glsl", "shaders/StarFragmentShader.glsl");
    boidShader = new Shader("shaders/BoidVertexShader.glsl", "shaders/FragmentShader.glsl");

    earth = new CelestialBody(10.0f, 16, 16);
    // Create the atmosphere with a radius of 1.0. It will be scaled up later using the model matrix
    atmosphere = new CelestialBody(1.0f, 32, 32);
    sun = new Star(2.0f, 50.0f, 0.5f, glm::vec3(1.0f, 0.95f, 0.9f));
    boidRenderer = new BoidRenderer();

    return true;
}

void GraphicsEngine::ProcessInput()
{
    ::ProcessInput(window);
}

bool GraphicsEngine::ShouldClose() const
{
    return glfwWindowShouldClose(window);
}

void GraphicsEngine::Render(const std::vector<glm::mat4> &boidData, bool drawSimulation, float simulationTime, float maxAltitude, float earthRadius)
{
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (drawSimulation)
    {
        // Update the sun with the new time (causes it to move in its orbit)
        sun->Update(simulationTime);

        glm::mat4 view = CalculateViewMatrix(camera);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Prevent division by zero if the window is minimized
        if (height == 0)
        {
            height = 1;
        }
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
        glm::vec3 lightDir = sun->GetDirection();
        glm::vec3 ambient(1.0f, 1.0f, 1.0f);

        // Draw Earth
        mainShader->use();
        mainShader->setMat4("view", view);
        mainShader->setMat4("projection", projection);
        glm::mat4 earthModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(earthRadius / 10.0f));
        // (Divide by 10.0f because the base CelestialBody was created with a radius of 10.0f)
        mainShader->setMat4("model", earthModelMatrix);
        glUniform3fv(glGetUniformLocation(mainShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(mainShader->ID, "ambientColor"), 1, glm::value_ptr(ambient));

        earth->Draw();

        // Draw boids
        boidShader->use();
        boidShader->setMat4("view", view);
        boidShader->setMat4("projection", projection);
        glUniform3fv(glGetUniformLocation(boidShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(boidShader->ID, "ambientColor"), 1, glm::value_ptr(ambient));
        boidRenderer->DrawInstanced(boidData);

        // Draw Sun
        sun->Draw(*sunShader, view, projection);

        // Draw atmosphere
        // Setup transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Prevent depth fighting and disable culling
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        atmosphereShader->use();
        atmosphereShader->setMat4("view", view);
        atmosphereShader->setMat4("projection", projection);

        glm::mat4 atmoModel = glm::scale(glm::mat4(1.0f), glm::vec3(maxAltitude));
        atmosphereShader->setMat4("model", atmoModel);

        // Extract the exact camera position from the view matrix
        // The 4th column (index 3) of an inverted view matrix is the world position.
        glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);

        glUniform3fv(glGetUniformLocation(atmosphereShader->ID, "viewPos"), 1, glm::value_ptr(camPos));
        glUniform3fv(glGetUniformLocation(atmosphereShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));

        atmosphere->Draw();

        // Reset OpenGL state so we don't break the opaque objects on the next frame
        glDepthMask(GL_TRUE);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
    }
}

GLFWwindow *GraphicsEngine::GetWindow() const
{
    return window;
}

void GraphicsEngine::SwapBuffers()
{
    // Application calls this after ImGui is done
    glfwSwapBuffers(window);
    glfwPollEvents();
}