#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>

#include "Camera.h"
#include "Shader.h"
#include "CelestialBody.h"
#include "Star.h"
#include "BoidRenderer.h"
#include "Window.h"
#include "Callbacks.h"

#include "stb_image.h"

class GraphicsEngine
{
public:
    GraphicsEngine();
    ~GraphicsEngine();

    bool Initialize(int, int, const char *);

    // The main visual operations
    void ProcessInput();
    bool ShouldClose() const;
    void Render(const std::vector<glm::mat4> &boidData, bool drawSimulation, float simulationTime, float maxAltitude, float earthRadius);
    GLFWwindow *GetWindow() const;
    void SwapBuffers();

    // Pass-through things for sun
    float GetSunOrbitDistance() const;
    void SetSunSpeed(float speed);

private:
    GLFWwindow *window;
    CameraState camera;

    Shader *earthShader;
    Shader *boidShader;
    Shader *sunShader;
    Shader *atmosphereShader;

    CelestialBody *earth;
    CelestialBody *atmosphere;
    Star *sun;
    BoidRenderer *boidRenderer;

    unsigned int earthTexture;
};

#endif