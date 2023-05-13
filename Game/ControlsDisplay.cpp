#include "ControlsDisplay.h"
#include <imgui.h>
#include <string>

using namespace engine;

void ControlsDisplay::RenderUI()
{
    ImGui::Begin("Controls");
    ImGui::SetWindowSize({ 360,300 }, ImGuiCond_FirstUseEver);
    ImGui::SetWindowPos({ 100,100 }, ImGuiCond_FirstUseEver);

    ImGui::Text("Up to 2 controllers supported");
    ImGui::Text("One player can also use keyboard");
    ImGui::Text("Sound plays when hitting a food element");

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Keyboard:");
    ImGui::Text("Move - WASD");
    ImGui::Text("Switch controlled character - Space");
    ImGui::Text("Die - 1");
    ImGui::Text("Reset lives - 2");

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Controller:");
    ImGui::Text("Move - Dpad or left joystick");
    ImGui::Text("Switch controlled character - A");
    ImGui::Text("Die - B");
    ImGui::Text("Reset lives - X");
    

    ImGui::End();
}
