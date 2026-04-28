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

    void ProcessInput();
    bool ShouldClose() const;
    void Render(const std::vector<glm::mat4> &boidData,
                const std::vector<glm::mat4> &huntingPredators,
                const std::vector<glm::mat4> &fullPredators,
                bool drawSimulation, float simulationTime,
                float maxAlt, float minAlt, float earthRadius);
    GLFWwindow *GetWindow() const;
    void SwapBuffers();

    float GetSunOrbitDistance() const;
    void SetSunSpeed(float speed);

    void ToggleColdWar(bool cw);

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

    bool isColdWar = false;
};

#endif