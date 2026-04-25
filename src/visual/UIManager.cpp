#include "UIManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

UIManager::UIManager() {}

UIManager::~UIManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::Initialize(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void UIManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UIManager::RenderSetupMenu(int &boidCount, float &earthRadius, float &minAlt, float &maxAlt, float sunOrbitDistance, float &sunSpeed)
{
    bool startClicked = false;

    float maxAllowedAlt = 100;

    ImGui::Begin("Simulation Setup");
    ImGui::Text("Configure your starting parameters:");
    ImGui::Spacing();

    ImGui::SliderInt("Number of Boids", &boidCount, 100, 20000);
    ImGui::SliderFloat("Earth Radius", &earthRadius, 1.0f, 50.0f);

    if (minAlt < earthRadius)
        minAlt = earthRadius;
    if (maxAlt < minAlt)
        maxAlt = minAlt;
    if (maxAlt > maxAllowedAlt)
    {
        maxAlt = maxAllowedAlt;
        if (minAlt > maxAlt)
            minAlt = maxAlt;
        if (earthRadius > minAlt)
            earthRadius = minAlt;
    }

    ImGui::SliderFloat("Min Altitude", &minAlt, earthRadius, maxAlt);
    ImGui::SliderFloat("Max Altitude", &maxAlt, minAlt, maxAllowedAlt);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.0f, 5.0f);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Reset to Defaults", ImVec2(200, 30)))
    {
        boidCount = 5000;
        earthRadius = 10.0f;
        minAlt = 10.0f;
        maxAlt = 15.0f;
        sunSpeed = 0.5f;
    }

    ImGui::Spacing();

    if (ImGui::Button("START SIMULATION", ImVec2(200, 50)))
        startClicked = true;

    ImGui::End();
    return startClicked;
}

void UIManager::RenderSimulationOverlay(float &cohesion, float &separation, float &alignment,
                                         float &visualRange, float &maxSpeed, float &maxForce)
{
    ImGui::Begin("Simulation Controls");
    ImGui::Text("Boid Behavior");
    ImGui::Spacing();

    ImGui::SliderFloat("Cohesion",     &cohesion,    0.0f, 5.0f);
    ImGui::SliderFloat("Separation",   &separation,  0.0f, 5.0f);
    ImGui::SliderFloat("Alignment",    &alignment,   0.0f, 5.0f);
    ImGui::SliderFloat("Visual Range", &visualRange, 0.1f, 20.0f);
    ImGui::SliderFloat("Max Speed",    &maxSpeed,    0.1f, 20.0f);
    ImGui::SliderFloat("Max Force",    &maxForce,    0.0f, 5.0f);

    ImGui::End();
}

void UIManager::RenderPauseMenu(bool &resume, bool &setup, bool &quit)
{
    resume = false;
    setup = false;
    quit = false;

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    ImGui::Begin("Paused", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);
    ImGui::Text("Simulation Paused");
    ImGui::Spacing();

    if (ImGui::Button("Resume", ImVec2(200, 50)))
        resume = true;
    if (ImGui::Button("Setup", ImVec2(200, 50)))
        setup = true;
    if (ImGui::Button("Quit", ImVec2(200, 50)))
        quit = true;

    ImGui::End();
}