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
    sunShader = new Shader("shaders/VertexShader.glsl", "shaders/StarFragmentShader.glsl");
    boidShader = new Shader("shaders/BoidVertexShader.glsl", "shaders/FragmentShader.glsl");

    earth = new CelestialBody(10.0f, 16, 16);
    sun = new Star(2.0f, 40.0f, 0.5f, glm::vec3(1.0f, 0.95f, 0.9f));
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

void GraphicsEngine::Render(const std::vector<glm::mat4> &boidData, bool drawSimulation)
{
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (drawSimulation)
    {
        // Update the visual sun (purely aesthetic, doesn't affect physics)
        sun->Update(glfwGetTime());

        glm::mat4 view = CalculateViewMatrix(camera);
        // The final parameter of perspective is the render distance
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 1000.0f);
        glm::vec3 lightDir = sun->GetDirection();
        glm::vec3 ambient(1.0f, 1.0f, 1.0f);

        // Draw Earth
        mainShader->use();
        mainShader->setMat4("view", view);
        mainShader->setMat4("projection", projection);
        mainShader->setMat4("model", glm::mat4(1.0f));
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