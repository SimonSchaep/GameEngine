#include "NameInputBox.h"
#include "Renderer.h"
#include <imgui.h>
#include <imgui_stdlib.h>

using namespace engine;

NameInputBox::NameInputBox(engine::GameObject* pGameObject)
    : UIRenderComponent(pGameObject)
{
    m_OnNameEntered = std::make_unique<Event<EventType, const std::string&>>();
}

void NameInputBox::RenderUI()
{
    auto windowSize = Renderer::GetInstance().GetWindowSize();

    bool isOpen = true;
    ImGui::Begin("NameInputBox", &isOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowSize({ 300,40 });
    ImGui::SetWindowPos({ GetTransform()->GetWorldPosition().x,windowSize.y - GetTransform()->GetWorldPosition().y });
    
    ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.1f,0.1f,0.1f,1 });
    ImGui::InputTextWithHint("##InputName", "Enter your name", &m_EnteredName);
    ImGui::PopStyleColor(1);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.15f,0.15f,0.15f,1 });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.1f,0.1f,0.1f,1 });
    ImGui::PushStyleColor(ImGuiCol_Button, { 0.125f,0.125f,0.125f,1 });
    m_ButtonPressed = ImGui::Button("Submit", {60,20});
    ImGui::PopStyleColor(3);

    ImGui::End();
}

void NameInputBox::Update()
{
    if (m_ButtonPressed) //in update since we don't want too much logic happening in renderUI
    {
        m_OnNameEntered->NotifyObservers(EventType::nameEntered, m_EnteredName);
        m_ButtonPressed = false;
        m_EnteredName = "";
    }
}
