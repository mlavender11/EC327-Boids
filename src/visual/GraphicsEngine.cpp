#include "GraphicsEngine.h"
#include "TextureLoader.h"
#include <algorithm>

GraphicsEngine::GraphicsEngine()
    : window(nullptr),
      earthShader(nullptr),
      sunShader(nullptr),
      boidShader(nullptr),
      atmosphereShader(nullptr),
      earth(nullptr),
      atmosphere(nullptr),
      sun(nullptr),
      boidRenderer(nullptr) {}

GraphicsEngine::~GraphicsEngine()
{
    delete earthShader;
    delete sunShader;
    delete boidShader;
    delete atmosphereShader;
    delete earth;
    delete atmosphere;
    delete sun;
    delete boidRenderer;

    glDeleteTextures(1, &earthTexture);

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

    earthShader = new Shader("shaders/EarthVertexShader.glsl", "shaders/EarthFragmentShader.glsl");
    boidShader = new Shader("shaders/BoidVertexShader.glsl", "shaders/BoidFragmentShader.glsl");
    atmosphereShader = new Shader("shaders/AtmosphereVertexShader.glsl", "shaders/AtmosphereFragmentShader.glsl");
    sunShader = new Shader("shaders/StarVertexShader.glsl", "shaders/StarFragmentShader.glsl");

    earth = new CelestialBody(10.0f, 24, 24);
    atmosphere = new CelestialBody(1.0f, 32, 32);
    sun = new Star(50.0f, 400.0f, 0.5f, glm::vec3(1.0f, 0.95f, 0.5f));
    boidRenderer = new BoidRenderer();

    earthTexture = TextureLoader::Load("assets/earth_crystal2.jpg");

    glEnable(GL_DEPTH_TEST);

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

void GraphicsEngine::Render(const std::vector<glm::mat4> &boidData,
                            const std::vector<glm::mat4> &predatorData,
                            bool drawSimulation, float simulationTime,
                            float maxAltitude, float minAltitude, float earthRadius)
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
        if (height == 0)
            height = 1;

        glViewport(0, 0, width, height);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 3000.0f);

        glm::vec3 lightDir = sun->GetDirection();
        glm::vec3 ambient(1.0f, 1.0f, 1.0f);

        // --- 1. Draw Earth ---
        earthShader->use();
        earthShader->setInt("u_ColdWar", isColdWar);
        earthShader->setMat4("view", view);
        earthShader->setMat4("projection", projection);

        glm::mat4 earthModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(earthRadius / 10.0f));
        earthShader->setMat4("model", earthModelMatrix);

        glUniform3fv(glGetUniformLocation(earthShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(earthShader->ID, "ambientColor"), 1, glm::value_ptr(ambient));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        earthShader->setInt("earthTextureMap", 0);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CW);
        earth->Draw();
        glFrontFace(GL_CCW);

        if (isColdWar)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // --- 2. Draw Boids (gold) and Predators (red) ---
        boidShader->use();
        boidShader->setInt("u_ColdWar", isColdWar);
        boidShader->setMat4("view", view);
        boidShader->setMat4("projection", projection);
        glUniform3fv(glGetUniformLocation(boidShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(boidShader->ID, "ambientColor"), 1, glm::value_ptr(ambient));

        if (isColdWar)
        {
            boidRenderer->DrawInstancedColored(boidData, glm::vec3(0.2f, 0.6f, 1.0f));     // Blue boids
            boidRenderer->DrawInstancedColored(predatorData, glm::vec3(1.0f, 0.1f, 0.1f)); // Red predators
        }
        else
        {
            // Standard lighting colors for when the theme is turned off
            boidRenderer->DrawInstanced(boidData);                                         // Default is yellow
            boidRenderer->DrawInstancedColored(predatorData, glm::vec3(1.0f, 0.1f, 0.1f)); // Red predators
        }

        // --- 3. Draw Sun ---
        sun->Draw(*sunShader, view, projection);

        // --- 4. Draw Boundaries ---
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);

        atmosphereShader->use();
        atmosphereShader->setInt("u_ColdWar", isColdWar);
        atmosphereShader->setMat4("view", view);
        atmosphereShader->setMat4("projection", projection);

        glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
        glUniform3fv(glGetUniformLocation(atmosphereShader->ID, "viewPos"), 1, glm::value_ptr(camPos));
        glUniform3fv(glGetUniformLocation(atmosphereShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));

        float outerVisualRadius = maxAltitude + 0.5f;
        glm::mat4 outerModel = glm::scale(glm::mat4(1.0f), glm::vec3(outerVisualRadius));
        atmosphereShader->setMat4("model", outerModel);
        atmosphere->Draw();

        float innerVisualRadius = std::max(minAltitude, earthRadius + 0.1f);
        glm::mat4 innerModel = glm::scale(glm::mat4(1.0f), glm::vec3(innerVisualRadius));
        atmosphereShader->setMat4("model", innerModel);
        atmosphere->Draw();

        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

GLFWwindow *GraphicsEngine::GetWindow() const { return window; }

void GraphicsEngine::SwapBuffers()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

float GraphicsEngine::GetSunOrbitDistance() const { return sun->GetOrbitDistance(); }

void GraphicsEngine::SetSunSpeed(float speed)
{
    if (sun)
        sun->SetSpeed(speed);
}

void GraphicsEngine::ToggleColdWar(bool cw)
{
    isColdWar = cw;
    if (cw)
        earthTexture = TextureLoader::Load("assets/earth_WarGames.jpg");
    else
        earthTexture = TextureLoader::Load("assets/earth_crystal2.jpg");
}