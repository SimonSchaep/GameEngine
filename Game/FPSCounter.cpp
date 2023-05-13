#include "FPSCounter.h"
#include "TextRenderComponent.h"
#include "GameObject.h"
#include "TimeManager.h"

using namespace engine;

void FPSCounter::Initialize()
{
	m_pTextRenderComponent = GetGameObject()->GetComponent<TextRenderComponent>();
}

void FPSCounter::Update()
{
	const std::string text{ "FPS: " + std::to_string(int(glm::round(1.f / TimeManager::GetInstance().GetDeltaTime()))) };
	m_pTextRenderComponent->SetText(text);
}
