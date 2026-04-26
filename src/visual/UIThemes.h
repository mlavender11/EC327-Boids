#ifndef UI_THEMES_H
#define UI_THEMES_H

#include <imgui.h>

namespace UIThemes
{

    // Helper to apply the Modern Slate style
    inline void ApplyModern(ImGuiStyle &style)
    {
        // Concentricity: High rounding for windows, capsule shapes for controls
        style.WindowRounding = 16.0f;
        style.FrameRounding = 12.0f;
        style.GrabRounding = 12.0f;
        style.PopupRounding = 12.0f;

        // Specular Highlights: Thin borders simulate light hitting the glass edge
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;

        ImVec4 *colors = style.Colors;

        // Translucent material that lets the background show through
        colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.55f);
        colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.15f); // Soft glass edge

        // Title bar with slightly more opacity to make it stand out
        colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.55f);       // Matches the glassy window
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.14f, 0.85f); // Slightly more opaque when clicked
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12f, 0.12f, 0.14f, 0.35f);

        // High contrast text to maintain legibility over the glass
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        // Controls (sliders, inputs) - slightly more opaque glass wells
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.50f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 0.60f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.70f);

        // Vibrant tints to emphasize primary elements and actions
        colors[ImGuiCol_Button] = ImVec4(0.00f, 0.48f, 1.00f, 0.75f); // Translucent Apple Blue
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.00f, 0.55f, 1.00f, 0.90f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.40f, 0.90f, 1.00f);

        colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.48f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        colors[ImGuiCol_Header] = ImVec4(0.00f, 0.48f, 1.00f, 0.50f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.48f, 1.00f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.48f, 1.00f, 1.00f);
    }

    // Helper to apply the Neon Synthwave style
    inline void ApplySynthwave(ImGuiStyle &style)
    {
        ImGui::GetStyle() = ImGuiStyle(); // Start with default style to ensure all properties are set
        ImVec4 *colors = style.Colors;

        // Sharp, aggressive geometry
        style.WindowRounding = 0.0f;
        style.FrameRounding = 2.0f;
        style.GrabRounding = 2.0f;

        // Thick borders to simulate neon tubes
        style.WindowBorderSize = 2.0f;
        style.FrameBorderSize = 1.0f;

        // --- The Synthwave Color Palette ---
        ImVec4 deepSpaceBlue = ImVec4(0.04f, 0.04f, 0.10f, 1.00f); // Very dark navy
        ImVec4 hotPink = ImVec4(1.00f, 0.00f, 0.54f, 1.00f);       // Aggressive neon pink
        ImVec4 cyberCyan = ImVec4(0.00f, 0.90f, 0.90f, 1.00f);     // Bright teal/cyan
        ImVec4 neonPurple = ImVec4(0.40f, 0.00f, 0.80f, 1.00f);    // Deep violet
        ImVec4 darkPurple = ImVec4(0.15f, 0.05f, 0.25f, 1.00f);    // Subdued purple for wells
        ImVec4 brightWhite = ImVec4(0.95f, 0.95f, 1.00f, 1.00f);   // Crisp readable text

        // Background & Borders
        colors[ImGuiCol_WindowBg] = deepSpaceBlue;
        colors[ImGuiCol_Border] = hotPink; // Pink outer "neon tube"

        // Text
        colors[ImGuiCol_Text] = cyberCyan; // Keep text readable

        // Title Bar (Changes from purple to pink when window is focused)
        colors[ImGuiCol_TitleBg] = darkPurple;
        colors[ImGuiCol_TitleBgActive] = hotPink;
        colors[ImGuiCol_TitleBgCollapsed] = deepSpaceBlue;

        // Frames (The background of sliders and text inputs)
        colors[ImGuiCol_FrameBg] = darkPurple;
        colors[ImGuiCol_FrameBgHovered] = neonPurple;
        colors[ImGuiCol_FrameBgActive] = hotPink;

        // Buttons (Dark Purple -> Pink on Hover -> Cyan Flash on Click!)
        colors[ImGuiCol_Button] = darkPurple;
        colors[ImGuiCol_ButtonHovered] = hotPink;
        colors[ImGuiCol_ButtonActive] = cyberCyan;

        // Sliders (Cyan grabber that turns pure white when dragged)
        colors[ImGuiCol_SliderGrab] = cyberCyan;
        colors[ImGuiCol_SliderGrabActive] = brightWhite;

        // Radio Buttons & Checkboxes
        colors[ImGuiCol_CheckMark] = cyberCyan;

        // Separators (The lines between UI sections)
        colors[ImGuiCol_Separator] = neonPurple;
        colors[ImGuiCol_SeparatorHovered] = hotPink;
        colors[ImGuiCol_SeparatorActive] = cyberCyan;
    }

    // Helper to apply the Matrix Retro style
    inline void ApplyRetro(ImGuiStyle &style)
    {
        ImGui::GetStyle() = ImGuiStyle(); // Start with default style to ensure all properties are set
        ImVec4 *colors = style.Colors;

        // --- Sharp Retro Settings ---
        style.WindowRounding = 0.0f; // Hard corners only
        style.FrameRounding = 0.0f;
        style.GrabRounding = 0.0f;
        style.WindowBorderSize = 1.0f; // Thin outline
        style.FrameBorderSize = 1.0f;

        // --- Matrix/CRT Color Palette ---
        ImVec4 neonGreen = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
        ImVec4 darkGreen = ImVec4(0.00f, 0.20f, 0.00f, 1.00f);
        ImVec4 black = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

        colors[ImGuiCol_Text] = neonGreen;
        colors[ImGuiCol_WindowBg] = black;
        colors[ImGuiCol_Border] = neonGreen;

        // Sliders & Frames
        colors[ImGuiCol_FrameBg] = black;
        colors[ImGuiCol_FrameBgHovered] = darkGreen;
        colors[ImGuiCol_SliderGrab] = neonGreen;
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.80f, 0.00f, 1.00f);

        // Titles & Headers
        colors[ImGuiCol_TitleBg] = black;
        colors[ImGuiCol_TitleBgActive] = darkGreen;
        colors[ImGuiCol_Header] = darkGreen;
        colors[ImGuiCol_HeaderHovered] = neonGreen;
        colors[ImGuiCol_HeaderActive] = neonGreen;
    }
}

#endif