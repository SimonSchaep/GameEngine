#include "FPSCounter.h"
#include "UITextRenderComponent.h"
#include "GameObject.h"
#include "TimeManager.h"

using namespace engine;

void FPSCounter::Initialize()
{
	m_UITextRenderComponent = GetGameObject()->GetComponent<UITextRenderComponent>();
}

void FPSCounter::Update()
{
	if (TimeManager::GetInstance().GetDeltaTime() < 0.001f) return;
	const std::string text{ "FPS: " + std::to_string(int(glm::round(1.f / TimeManager::GetInstance().GetDeltaTime()))) };
	m_UITextRenderComponent->SetText(text);
}
