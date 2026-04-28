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

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontDefault();

    modernFont  = io.Fonts->AddFontFromFileTTF("assets/fonts/SF-Pro-Text-Regular.otf", 18.0f);
    synthFont   = io.Fonts->AddFontFromFileTTF("assets/fonts/Alien-Encounters-Regular.ttf", 17.0f);
    cwFont      = io.Fonts->AddFontFromFileTTF("assets/fonts/WarGamesTerminalRT.ttf", 18.0f);
    falloutFont = io.Fonts->AddFontFromFileTTF("assets/fonts/JH_Fallout.ttf", 17.0f);

    ApplyTheme(activeTheme);
}

void UIManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImFont *fontToUse = nullptr;
    if (activeTheme == UITheme::MODERN)
        fontToUse = modernFont;
    else if (activeTheme == UITheme::SYNTHWAVE)
        fontToUse = synthFont;
    else if (activeTheme == UITheme::COLDWAR)
        fontToUse = cwFont;
    else if (activeTheme == UITheme::FALLOUT)
        fontToUse = falloutFont;

    if (fontToUse != nullptr)
    {
        ImGui::PushFont(fontToUse);
        fontPushedThisFrame = true;
    }
    else
    {
        fontPushedThisFrame = false;
    }
}

void UIManager::EndFrame()
{
    if (fontPushedThisFrame)
    {
        ImGui::PopFont();
        fontPushedThisFrame = false;
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// *** CHANGED: added int &predatorCount parameter ***
bool UIManager::RenderSetupMenu(int &boidCount, float &earthRadius, float &minAlt, float &maxAlt, float sunOrbitDistance, float &sunSpeed, int &predatorCount, bool &quit)
{
    bool startClicked = false;
    ImGuiIO &io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(io.DisplaySize.x, io.DisplaySize.y));

    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Simulation Setup", nullptr, flags);

    float maxAllowedAlt = 100;

    ImGui::Text("Configure your starting parameters:");
    ImGui::Spacing();

    ImGui::SliderInt("Number of Boids",     &boidCount,     100, 2000);
    ImGui::SliderInt("Number of Predators", &predatorCount, 0,   10);  // *** NEW ***
    ImGui::SliderFloat("Earth Radius", &earthRadius, 1.0f, 50.0f);

    if (minAlt < earthRadius) minAlt = earthRadius;
    if (maxAlt < minAlt)      maxAlt = minAlt;
    if (maxAlt > maxAllowedAlt)
    {
        maxAlt = maxAllowedAlt;
        if (minAlt > maxAlt)      minAlt = maxAlt;
        if (earthRadius > minAlt) earthRadius = minAlt;
    }

    ImGui::SliderFloat("Min Altitude",    &minAlt,   earthRadius, maxAlt);
    ImGui::SliderFloat("Max Altitude",    &maxAlt,   minAlt, maxAllowedAlt);
    ImGui::SliderFloat("Day/Night Speed", &sunSpeed, 0.0f, 2.0f);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Reset to Defaults", ImVec2(200, 30)))
    {
        boidCount      = 500;
        predatorCount  = 3;    // *** NEW ***
        earthRadius    = 5.0f;
        minAlt         = 5.0f;
        maxAlt         = 10.0f;
        sunSpeed       = 0.3f;
    }

    ImGui::SameLine();

    if (ImGui::Button("Start Simulation", ImVec2(200, 30)))
        startClicked = true;

    ImGui::Spacing();
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 200) * 0.5f);
    if (ImGui::Button("Quit Game", ImVec2(200, 30)))
        quit = true;

    ImGui::End();
    return startClicked;
}

void UIManager::RenderPauseMenu(bool &resume, bool &setup, bool &quit, bool &graphics)
{
    resume = false;
    setup = false;
    quit = false;
    graphics = false;

    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, -1), ImVec2(io.DisplaySize.x, io.DisplaySize.y));

    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    ImGui::Begin("Paused", nullptr, flags);
    ImGui::Spacing();

    if (ImGui::Button("Resume",          ImVec2(200, 40))) resume   = true;
    if (ImGui::Button("GUI Theme",       ImVec2(200, 40))) graphics = true;
    if (ImGui::Button("Return to Setup", ImVec2(200, 40))) setup    = true;
    if (ImGui::Button("Quit Game",       ImVec2(200, 40))) quit     = true;

    ImGui::End();
}

void UIManager::RenderSimulationOverlay(float &cohesion, float &separation, float &alignment, float &visualRange, float &maxSpeed, float &maxForce, float simulationTime)
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImGui::Begin("Flocking Controls", nullptr, windowFlags);

    ImGui::Text("Boid Behavior Parameters");
    ImGui::Separator();

    ImGui::SliderFloat("Separation",   &separation,  0.0f, 5.0f);
    ImGui::SliderFloat("Alignment",    &alignment,   0.0f, 5.0f);
    ImGui::SliderFloat("Cohesion",     &cohesion,    0.0f, 5.0f);
    ImGui::SliderFloat("Visual Range", &visualRange, 1.0f, 20.0f);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::Text("Physical Limits");
    ImGui::SliderFloat("Max Speed",         &maxSpeed, 0.1f, 30.0f);
    ImGui::SliderFloat("Max Turning Force", &maxForce, 0.1f, 30.0f);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Reset to Defaults", ImVec2(200, 30)))
    {
        cohesion    = 1.0f;
        separation  = 1.0f;
        alignment   = 1.0f;
        visualRange = 5.0f;
        maxSpeed    = 5.0f;
        maxForce    = 10.0f;
    }

    if (simulationTime < 10.0f && !promptDismissed)
    {
        ImGuiIO &io = ImGui::GetIO();
        ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.85f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

        ImGuiWindowFlags promptFlags = ImGuiWindowFlags_NoDecoration |
                                       ImGuiWindowFlags_AlwaysAutoResize |
                                       ImGuiWindowFlags_NoMove |
                                       ImGuiWindowFlags_NoSavedSettings |
                                       ImGuiWindowFlags_NoFocusOnAppearing;

        ImGui::Begin("ControlsPrompt", nullptr, promptFlags);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::Text("CONTROLS: Click and Drag to Rotate, Scroll to Zoom, Escape for Menu");
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            promptDismissed = true;
        ImGui::PopStyleColor();
        ImGui::End();
    }

    ImGui::End();
}

void UIManager::ApplyTheme(UITheme theme)
{
    activeTheme = theme;
    ImGuiStyle &style = ImGui::GetStyle();

    switch (theme)
    {
    case UITheme::MODERN:    UIThemes::ApplyModern(style);    break;
    case UITheme::SYNTHWAVE: UIThemes::ApplySynthwave(style); break;
    case UITheme::COLDWAR:   UIThemes::ApplyColdWar(style);   break;
    case UITheme::FALLOUT:   UIThemes::ApplyFallout(style);   break;
    case UITheme::CLASSIC:   ImGui::StyleColorsDark();         break;
    }
}

void UIManager::RenderGraphicsMenu(UITheme &currentTheme, bool &backClicked)
{
    ImGuiIO &io = ImGui::GetIO();
    ImGui::SetNextWindowSizeConstraints(ImVec2(400, -1), ImVec2(io.DisplaySize.x, io.DisplaySize.y));
    ImGui::Begin("GUI Theme", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::RadioButton("Modern Dark", currentTheme == UITheme::MODERN))   { currentTheme = UITheme::MODERN;    ApplyTheme(UITheme::MODERN); }
    if (ImGui::RadioButton("Synthwave",   currentTheme == UITheme::SYNTHWAVE)) { currentTheme = UITheme::SYNTHWAVE; ApplyTheme(UITheme::SYNTHWAVE); }
    if (ImGui::RadioButton("WarGames",    currentTheme == UITheme::COLDWAR))   { currentTheme = UITheme::COLDWAR;   ApplyTheme(UITheme::COLDWAR); }
    if (ImGui::RadioButton("Fallout",     currentTheme == UITheme::FALLOUT))   { currentTheme = UITheme::FALLOUT;   ApplyTheme(UITheme::FALLOUT); }
    if (ImGui::RadioButton("Classic",     currentTheme == UITheme::CLASSIC))   { currentTheme = UITheme::CLASSIC;   ApplyTheme(UITheme::CLASSIC); }

    ImGui::Separator();
    if (ImGui::Button("Back"))
        backClicked = true;

    ImGui::End();
}

UITheme UIManager::GetActiveTheme() const { return activeTheme; }

void UIManager::ResetPromptDismissal() { promptDismissed = false; }