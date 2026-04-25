#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine()
    : window(nullptr),
      mainShader(nullptr),
      sunShader(nullptr),
      boidShader(nullptr),
      atmosphereShader(nullptr),
      earth(nullptr),
      atmosphere(nullptr),
      sun(nullptr),
      boidRenderer(nullptr) {}

GraphicsEngine::~GraphicsEngine()
{
    delete mainShader;
    delete sunShader;
    delete boidShader;
    delete atmosphereShader;
    delete earth;
    delete atmosphere;
    delete sun;
    delete boidRenderer;
    glfwTerminate();
}

bool GraphicsEngine::Initialize(int width, int height, const char *title)
{
    window = InitializeWindow(width, height, title);
    if (!window)
        return false;

    glfwSetWindowUserPointer(window, &camera);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    mainShader       = new Shader("shaders/VertexShader.glsl", "shaders/FragmentShader.glsl");
    atmosphereShader = new Shader("shaders/AtmosphereVertexShader.glsl", "shaders/AtmosphereFragmentShader.glsl");
    sunShader        = new Shader("shaders/VertexShader.glsl", "shaders/StarFragmentShader.glsl");
    boidShader       = new Shader("shaders/BoidVertexShader.glsl", "shaders/FragmentShader.glsl");

    earth        = new CelestialBody(10.0f, 24, 24);
    atmosphere   = new CelestialBody(1.0f, 32, 32);
    sun          = new Star(50.0f, 400.0f, 0.5f, glm::vec3(1.0f, 0.95f, 0.5f));
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

void GraphicsEngine::Render(const std::vector<glm::mat4>& friendlyData,
                             const std::vector<glm::mat4>& predatorData,
                             bool drawSimulation,
                             float simulationTime,
                             float maxAltitude,
                             float earthRadius)
{
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.minZoom = earthRadius + 2.0f;
    camera.maxZoom = maxAltitude * 3.0f + 20.0f;

    if (camera.radius < camera.minZoom)
        camera.radius = camera.minZoom;
    if (camera.radius > camera.maxZoom)
        camera.radius = camera.maxZoom;

    if (drawSimulation)
    {
        sun->Update(simulationTime);

        glm::mat4 view = CalculateViewMatrix(camera);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (height == 0) height = 1;

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 3000.0f);
        glm::vec3 lightDir = sun->GetDirection();
        glm::vec3 ambient(1.0f, 1.0f, 1.0f);

        // Draw Earth
        mainShader->use();
        mainShader->setMat4("view", view);
        mainShader->setMat4("projection", projection);
        glm::mat4 earthModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(earthRadius / 10.0f));
        mainShader->setMat4("model", earthModelMatrix);
        glUniform3fv(glGetUniformLocation(mainShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(mainShader->ID, "ambientColor"), 1, glm::value_ptr(ambient));
        earth->Draw();

        // Draw boids — friendlies green, predators red
        boidShader->use();
        boidShader->setMat4("view", view);
        boidShader->setMat4("projection", projection);
        glUniform3fv(glGetUniformLocation(boidShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(boidShader->ID, "ambientColor"), 1, glm::value_ptr(ambient));
        boidRenderer->DrawInstanced(friendlyData, glm::vec3(0.0f, 0.9f, 0.2f)); // green
        boidRenderer->DrawInstanced(predatorData, glm::vec3(0.9f, 0.1f, 0.1f)); // red

        // Draw Sun
        sun->Draw(*sunShader, view, projection);

        // Draw atmosphere
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        atmosphereShader->use();
        atmosphereShader->setMat4("view", view);
        atmosphereShader->setMat4("projection", projection);

        float atmosphereRadius = maxAltitude + 0.5f;
        glm::mat4 atmoModel = glm::scale(glm::mat4(1.0f), glm::vec3(atmosphereRadius));
        atmosphereShader->setMat4("model", atmoModel);

        glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
        glUniform3fv(glGetUniformLocation(atmosphereShader->ID, "viewPos"), 1, glm::value_ptr(camPos));
        glUniform3fv(glGetUniformLocation(atmosphereShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));

        atmosphere->Draw();

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
    glfwSwapBuffers(window);
    glfwPollEvents();
}

float GraphicsEngine::GetSunOrbitDistance() const
{
    return sun->GetOrbitDistance();
}

void GraphicsEngine::SetSunSpeed(float speed)
{
    sun->SetSpeed(speed);
}