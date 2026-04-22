#include "GraphicsEngine.h"

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
    {
        return false;
    }

    glfwSetWindowUserPointer(window, &camera);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    earthShader = new Shader("shaders/EarthVertexShader.glsl", "shaders/EarthFragmentShader.glsl");
    boidShader = new Shader("shaders/BoidVertexShader.glsl", "shaders/BoidFragmentShader.glsl");
    atmosphereShader = new Shader("shaders/AtmosphereVertexShader.glsl", "shaders/AtmosphereFragmentShader.glsl");
    sunShader = new Shader("shaders/StarVertexShader.glsl", "shaders/StarFragmentShader.glsl");

    // Radii for the Earth and atmosphere will be scaled by the user during setup
    earth = new CelestialBody(10.0f, 24, 24);
    atmosphere = new CelestialBody(1.0f, 32, 32);
    sun = new Star(50.0f, 400.0f, 0.5f, glm::vec3(1.0f, 0.95f, 0.5f));
    boidRenderer = new BoidRenderer();

    // -- Load Earth Texture --
    glGenTextures(1, &earthTexture);
    glBindTexture(GL_TEXTURE_2D, earthTexture);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(false);

    int imageWidth, imageHeight, nrChannels;
    unsigned char *data = stbi_load("assets/earth_crystal2.jpg", &imageWidth, &imageHeight, &nrChannels, 0);
    // Taken from https://svs.gsfc.nasa.gov/3615/

    if (data)
    {
        // If image is a PNG with transparency, change GL_RGB to GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load Earth texture" << std::endl;
    }
    stbi_image_free(data);

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

void GraphicsEngine::Render(const std::vector<glm::mat4> &boidData, bool drawSimulation, float simulationTime, float maxAltitude, float earthRadius)
{
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update camera bounds
    // Prevent clipping into the ground by adding a tiny buffer to the minZoom
    camera.minZoom = earthRadius + 2.0f;

    // Set max zoom to 3x the max altitude plus a buffer
    camera.maxZoom = maxAltitude * 3.0f + 20.0f;

    // Clamp the max and min zooms
    if (camera.radius < camera.minZoom)
    {
        camera.radius = camera.minZoom;
    }
    if (camera.radius > camera.maxZoom)
    {
        camera.radius = camera.maxZoom;
    }

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

        // This forces the OpenGL drawing area to match the window dimensions
        glViewport(0, 0, width, height);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 3000.0f);
        glm::vec3 lightDir = sun->GetDirection();
        glm::vec3 ambient(1.0f, 1.0f, 1.0f);

        // Draw Earth
        earthShader->use();
        earthShader->setMat4("view", view);
        earthShader->setMat4("projection", projection);
        glm::mat4 earthModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(earthRadius / 10.0f));
        // (Divide by 10.0f because the base CelestialBody was created with a radius of 10.0f)
        earthShader->setMat4("model", earthModelMatrix);
        glUniform3fv(glGetUniformLocation(earthShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(earthShader->ID, "ambientColor"), 1, glm::value_ptr(ambient));

        // Bind Earth texture to texture unit 0 and set the shader uniform to use it
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, earthTexture);
        earthShader->setInt("earthTextureMap", 0);

        glEnable(GL_CULL_FACE); // Turn on culling
        glFrontFace(GL_CW);     // Tell OpenGL that Clockwise means "Front"
        earth->Draw();
        glFrontFace(GL_CCW); // Reset to default (Counter-Clockwise is front) so that the atmosphere shader doesn't break

        earth->Draw();

        // Draw boids
        boidShader->use();
        boidShader->setMat4("view", view);
        boidShader->setMat4("projection", projection);
        glUniform3fv(glGetUniformLocation(boidShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(boidShader->ID, "ambientColor"), 1, glm::value_ptr(ambient));
        boidRenderer->DrawInstanced(boidData);

        // Draw Sun
        // the use(), setMat4(), and glUniform3fv() calls are all handled inside the Star::Draw() function
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

        float atmosphereRadius = maxAltitude + 0.5f; // Add a small offset to prevent boids from clipping into the atmosphere
        glm::mat4 atmoModel = glm::scale(glm::mat4(1.0f), glm::vec3(atmosphereRadius));
        atmosphereShader->setMat4("model", atmoModel);

        // Extract the exact camera position from the view matrix
        // The 4th column (index 3) of an inverted view matrix is the world position.
        glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);

        glUniform3fv(glGetUniformLocation(atmosphereShader->ID, "viewPos"), 1, glm::value_ptr(camPos));
        glUniform3fv(glGetUniformLocation(atmosphereShader->ID, "lightDir"), 1, glm::value_ptr(lightDir));

        atmosphere->Draw();

        // Reset OpenGL state so we don't break the opaque objects on the next frame
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);
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

float GraphicsEngine::GetSunOrbitDistance() const
{
    return sun->GetOrbitDistance();
}

void GraphicsEngine::SetSunSpeed(float speed)
{
    if (sun)
    {
        sun->SetSpeed(speed);
    }
}