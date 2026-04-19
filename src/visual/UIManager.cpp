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

bool UIManager::RenderSetupMenu(int &boidCount, float &earthRadius, float &minAlt, float &maxAlt)
{
    bool startClicked = false;

    ImGui::Begin("Simulation Setup");
    ImGui::Text("Configure your starting parameters:");
    ImGui::Spacing();

    // The sliders directly modify the variables passed in by reference
    ImGui::SliderInt("Number of Boids", &boidCount, 100, 20000);
    ImGui::SliderFloat("Earth Radius", &earthRadius, 1.0f, 50.0f);
    ImGui::SliderFloat("Min Altitude", &minAlt, earthRadius, 60.0f);
    ImGui::SliderFloat("Max Altitude", &maxAlt, minAlt, 100.0f);

    ImGui::Spacing();

    if (ImGui::Button("START SIMULATION", ImVec2(200, 50)))
    {
        startClicked = true;
    }
    ImGui::End();

    return startClicked;
}