#ifndef UI_THEMES_H
#define UI_THEMES_H

#include <imgui.h>

namespace UIThemes
{

    // Helper to apply the Modern Slate style
    inline void ApplyModern(ImGuiStyle &style)
    {
        // Start with default style to ensure all properties are set
        ImGui::StyleColorsDark();
        ImGui::GetStyle() = ImGuiStyle();
        ImVec4 *colors = style.Colors;

        // Concentricity: High rounding for windows, capsule shapes for controls
        style.WindowRounding = 16.0f;
        style.FrameRounding = 12.0f;
        style.GrabRounding = 12.0f;
        style.PopupRounding = 12.0f;

        // Specular Highlights: Thin borders simulate light hitting the glass edge
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;

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
        // Start with default style to ensure all properties are set
        ImGui::StyleColorsDark();
        ImGui::GetStyle() = ImGuiStyle();
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

    inline void ApplyColdWar(ImGuiStyle &style)
    {
        // Start with default style to ensure all properties are set
        ImGui::StyleColorsDark();
        ImGui::GetStyle() = ImGuiStyle();
        ImVec4 *colors = style.Colors;

        // Complete removal of rounded edges for that harsh, early-computing look
        style.WindowRounding = 0.0f;
        style.FrameRounding = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding = 0.0f;
        style.TabRounding = 0.0f;
        style.PopupRounding = 0.0f;

        // Distinct, sharp borders are crucial for this aesthetic
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 1.0f;
        style.PopupBorderSize = 1.0f;

        // The Monochrome Palette
        ImVec4 terminalBlack = ImVec4(0.00f, 0.00f, 0.00f, 1.00f); // Pure black
        ImVec4 phosphorWhite = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Crisp CRT white/gray
        ImVec4 highlightGray = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Mid-gray for selections

        // Backgrounds
        colors[ImGuiCol_WindowBg] = terminalBlack;
        colors[ImGuiCol_ChildBg] = terminalBlack;
        colors[ImGuiCol_PopupBg] = terminalBlack;

        // Borders
        colors[ImGuiCol_Border] = phosphorWhite;
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Text
        colors[ImGuiCol_Text] = phosphorWhite;
        colors[ImGuiCol_TextDisabled] = highlightGray;

        // Headers (Used for Menus, Collapsing Headers, and highlighted list items)
        colors[ImGuiCol_Header] = terminalBlack;
        colors[ImGuiCol_HeaderHovered] = highlightGray;
        colors[ImGuiCol_HeaderActive] = phosphorWhite;

        // Title Bars (Window headers like "- CONSOLE TERMINAL")
        colors[ImGuiCol_TitleBg] = terminalBlack;
        colors[ImGuiCol_TitleBgActive] = terminalBlack;
        colors[ImGuiCol_TitleBgCollapsed] = terminalBlack;

        // Frames (Checkboxes, Text Inputs, Sliders backgrounds)
        colors[ImGuiCol_FrameBg] = terminalBlack;
        colors[ImGuiCol_FrameBgHovered] = highlightGray;
        colors[ImGuiCol_FrameBgActive] = phosphorWhite;

        // Buttons
        colors[ImGuiCol_Button] = terminalBlack;
        colors[ImGuiCol_ButtonHovered] = highlightGray;
        colors[ImGuiCol_ButtonActive] = phosphorWhite;

        // Sliders and Scrollbars
        colors[ImGuiCol_ScrollbarBg] = terminalBlack;
        colors[ImGuiCol_ScrollbarGrab] = phosphorWhite;
        colors[ImGuiCol_ScrollbarGrabHovered] = highlightGray;
        colors[ImGuiCol_ScrollbarGrabActive] = phosphorWhite;

        colors[ImGuiCol_SliderGrab] = phosphorWhite;
        colors[ImGuiCol_SliderGrabActive] = phosphorWhite;
        colors[ImGuiCol_CheckMark] = phosphorWhite;

        // Text Selection
        colors[ImGuiCol_TextSelectedBg] = highlightGray;
    }

    inline void ApplyFallout(ImGuiStyle &style)
    {
        // Start with default style to ensure all properties are set
        ImGui::StyleColorsDark();
        ImGui::GetStyle() = ImGuiStyle();
        ImVec4 *colors = style.Colors;

        // Blocky terminal aesthetic
        style.WindowRounding = 0.0f;
        style.FrameRounding = 0.0f;
        style.ScrollbarRounding = 0.0f;
        style.GrabRounding = 0.0f;
        style.TabRounding = 0.0f;

        // Thick borders to simulate CRT scanline UI
        style.WindowBorderSize = 2.0f;
        style.FrameBorderSize = 1.0f;

        // Classic Pip-Boy Phosphor Green
        ImVec4 pipBoyGreen = ImVec4(0.12f, 0.88f, 0.12f, 1.00f);
        ImVec4 pipBoyGreenDim = ImVec4(0.06f, 0.44f, 0.06f, 1.00f);
        ImVec4 crtBackground = ImVec4(0.02f, 0.05f, 0.02f, 0.95f); // Very dark green-tinted black
        ImVec4 crtPanel = ImVec4(0.04f, 0.10f, 0.04f, 1.00f);

        // Apply the monochrome green look
        colors[ImGuiCol_Text] = pipBoyGreen;
        colors[ImGuiCol_TextDisabled] = pipBoyGreenDim;
        colors[ImGuiCol_WindowBg] = crtBackground;
        colors[ImGuiCol_Border] = pipBoyGreen;
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = crtPanel;
        colors[ImGuiCol_FrameBgHovered] = pipBoyGreenDim;
        colors[ImGuiCol_FrameBgActive] = pipBoyGreen;
        colors[ImGuiCol_TitleBg] = crtPanel;
        colors[ImGuiCol_TitleBgActive] = pipBoyGreenDim;
        colors[ImGuiCol_TitleBgCollapsed] = crtBackground;
        colors[ImGuiCol_MenuBarBg] = crtPanel;
        colors[ImGuiCol_ScrollbarBg] = crtBackground;
        colors[ImGuiCol_ScrollbarGrab] = pipBoyGreenDim;
        colors[ImGuiCol_ScrollbarGrabHovered] = pipBoyGreen;
        colors[ImGuiCol_ScrollbarGrabActive] = pipBoyGreen;
        colors[ImGuiCol_CheckMark] = pipBoyGreen;
        colors[ImGuiCol_SliderGrab] = pipBoyGreen;
        colors[ImGuiCol_SliderGrabActive] = pipBoyGreen;
        colors[ImGuiCol_Button] = crtPanel;
        colors[ImGuiCol_ButtonHovered] = pipBoyGreenDim;
        colors[ImGuiCol_ButtonActive] = pipBoyGreen;
        colors[ImGuiCol_Header] = crtPanel;
        colors[ImGuiCol_HeaderHovered] = pipBoyGreenDim;
        colors[ImGuiCol_HeaderActive] = pipBoyGreen;

        // Make text selections and separators match the theme
        colors[ImGuiCol_Separator] = pipBoyGreen;
        colors[ImGuiCol_SeparatorHovered] = pipBoyGreen;
        colors[ImGuiCol_SeparatorActive] = pipBoyGreen;
        colors[ImGuiCol_TextSelectedBg] = pipBoyGreenDim;
    }
}

#endif