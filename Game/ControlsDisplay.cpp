#include "ControlsDisplay.h"
#include <imgui.h>
#include <string>

using namespace engine;

void ControlsDisplay::RenderUI()
{
    ImGui::Begin("Controls");
    ImGui::SetWindowSize({ 360,300 }, ImGuiCond_FirstUseEver);
    ImGui::SetWindowPos({ 100,100 }, ImGuiCond_FirstUseEver);

    ImGui::Text("Press SPACE to start");
    ImGui::Text("Press P to pause/unpause");
    ImGui::Text("Press G to simulate end of game");
    ImGui::Text("and show leaderboard");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Up to 2 controllers supported");
    ImGui::Text("One player can also use keyboard");
    ImGui::Text("Sound plays when hitting a food element");

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Keyboard:");
    ImGui::Text("Move - WASD");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Controller:");
    ImGui::Text("Move - Dpad or left joystick");
    

    ImGui::End();
}
