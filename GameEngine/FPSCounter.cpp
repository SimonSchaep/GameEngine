#include "FPSCounter.h"
#include "TextRenderComponent.h"
#include "GameObject.h"
#include "Time.h"

void dae::FPSCounter::Initialize()
{
	m_TextRenderComponent = m_GameObject->GetComponent<TextRenderComponent>();
}

void dae::FPSCounter::Update()
{
	std::string text{"FPS: " + std::to_string(1.f / Time::GetInstance().GetDeltaTime())};
	m_TextRenderComponent->SetText(text);
}
