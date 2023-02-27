#include "FPSCounter.h"
#include "TextRenderComponent.h"
#include "GameObject.h"
#include "Time.h"

void FPSCounter::Initialize()
{
	m_pTextRenderComponent = GetGameObject()->GetComponent<TextRenderComponent>();
}

void FPSCounter::Update()
{
	const std::string text{ "FPS: " + std::to_string(int(glm::round(1.f / Time::GetInstance().GetDeltaTime()))) };
	m_pTextRenderComponent->SetText(text);
}
