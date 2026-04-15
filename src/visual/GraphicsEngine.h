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

class GraphicsEngine
{
public:
    GraphicsEngine();
    ~GraphicsEngine();

    bool Initialize(int width, int height, const char *title);

    // The main visual operations
    void ProcessInput();
    bool ShouldClose() const;
    void Render(const std::vector<glm::mat4> &boidData);

private:
    GLFWwindow *window;
    CameraState camera;

    Shader *mainShader;
    Shader *sunShader;
    Shader *boidShader;

    CelestialBody *earth;
    Star *sun;
    BoidRenderer *boidRenderer;
};

#endif