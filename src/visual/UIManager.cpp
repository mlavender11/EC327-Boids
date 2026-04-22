#include "UIManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

UIManager::UIManager() {}

UIManager::~UIManager()
{
    // Clean up ImGui when the program closes
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::Initialize(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    // ImGui::StyleColorsDark();
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

    float maxAllowedAlt = 100; // Arbitrary

    ImGui::Begin("Simulation Setup");
    ImGui::Text("Configure your starting parameters:");
    ImGui::Spacing();

    ImGui::SliderInt("Number of Boids", &boidCount, 100, 2000);
    ImGui::SliderFloat("Earth Radius", &earthRadius, 1.0f, 50.0f);

    // --- CASCADING CLAMP LOGIC ---

    // 1. Earth pushes Min Up
    if (minAlt < earthRadius)
        minAlt = earthRadius;

    // 2. Min pushes Max Up
    if (maxAlt < minAlt)
        maxAlt = minAlt;

    // 3. Max hits the limit, and pushes everything back down
    if (maxAlt > maxAllowedAlt)
    {
        maxAlt = maxAllowedAlt;

        if (minAlt > maxAlt)
        {
            minAlt = maxAlt;
        }
        if (earthRadius > minAlt)
        {
            earthRadius = minAlt;
        }
    }

    // By passing the dynamically checked variables into the min/max parameters
    // of the ImGui sliders, the sliders will visually stop dragging.
    ImGui::SliderFloat("Min Altitude", &minAlt, earthRadius, maxAlt);
    ImGui::SliderFloat("Max Altitude", &maxAlt, minAlt, maxAllowedAlt);
    ImGui::SliderFloat("Day/Night Speed", &sunSpeed, 0.0f, 2.0f);

    // --- RESET BUTTON ---
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Reset to Defaults", ImVec2(200, 30)))
    {
        boidCount = 500;
        earthRadius = 10.0f;
        minAlt = 10.0f;
        maxAlt = 20.0f;
        sunSpeed = 0.3f;
    }

    ImGui::Spacing();

    if (ImGui::Button("START SIMULATION", ImVec2(200, 50)))
    {
        startClicked = true;
    }
    ImGui::End();

    return startClicked;
}

void UIManager::RenderPauseMenu(bool &resume, bool &setup, bool &quit)
{
    resume = false;
    setup = false;
    quit = false;

    // Center the pause menu on the screen
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

void UIManager::RenderSimulationOverlay(float &cohesion, float &separation, float &alignment, float &visualRange, float &maxSpeed, float &maxForce)
{
    // Position the window in the top left corner automatically
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

    // Create a window without a background or title bar for a cleaner look
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::SetNextWindowBgAlpha(0.6f); // Semi-transparent background

    ImGui::Begin("Flocking Controls", nullptr, windowFlags);
    ImGui::Text("Real-Time Flocking Weights");
    ImGui::Separator();

    // Sliders to tweak the math live
    ImGui::SliderFloat("Separation", &separation, 0.0f, 5.0f);
    ImGui::SliderFloat("Alignment", &alignment, 0.0f, 5.0f);
    ImGui::SliderFloat("Cohesion", &cohesion, 0.0f, 5.0f);
    ImGui::SliderFloat("Visual Range", &visualRange, 1.0f, 20.0f);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Physical Limits");
    ImGui::SliderFloat("Max Speed", &maxSpeed, 0.1f, 30.0f);
    ImGui::SliderFloat("Max Turning Force", &maxForce, 0.1f, 30.0f);

    // --- RESET BUTTON ---
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Reset to Defaults", ImVec2(200, 30)))
    {
        cohesion = 1.0f;
        separation = 1.0f;
        alignment = 1.0f;
        visualRange = 5.0f;
        maxSpeed = 5.0f;
        maxForce = 10.0f;
    }

    ImGui::End();
}