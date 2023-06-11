#include "ControlsDisplay.h"
#include <imgui.h>
#include <string>

using namespace engine;

void ControlsDisplay::RenderUI()
{
    ImGui::Begin("Controls");
    ImGui::SetWindowSize({ 260,300 }, ImGuiCond_Once);
    ImGui::SetWindowPos({ 700,55 }, ImGuiCond_Once);
    ImGui::SetWindowCollapsed(true, ImGuiCond_Once);

    ImGui::Text("Press 1/2/3 to start");
    ImGui::Text("Press P to pause/unpause");
    ImGui::Text("Press Escape to go back to the menu");
    ImGui::Text("Press F1 to go to the next level");
    ImGui::Text("Press F2 to simulate end of game");
    ImGui::Text("Press F3 to mute/unmute");
    ImGui::Text("and show leaderboard");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Up to 2 controllers supported");
    ImGui::Text("One player can also use keyboard");

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Keyboard:");
    ImGui::Text("Move - WASD or arrow keys");
    ImGui::Text("Pepper - SPACE");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Controller:");
    ImGui::Text("Move - Left thumbstick or dpad");
    ImGui::Text("Pepper - A");
    

    ImGui::End();
}
