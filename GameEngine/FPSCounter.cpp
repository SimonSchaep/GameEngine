#include "FPSCounter.h"
#include "TextRenderComponent.h"
#include "GameObject.h"
#include "Time.h"

void dae::FPSCounter::Initialize()
{
	m_pTextRenderComponent = m_pGameObject->GetComponent<TextRenderComponent>();
}

void dae::FPSCounter::Update()
{
	const std::string text{ "FPS: " + std::to_string(int(glm::round(1.f / Time::GetInstance().GetDeltaTime()))) };
	m_pTextRenderComponent->SetText(text);

	//to test removal of components ang gameobjects
	/*if (Time::GetInstance().GetTotalTime() > 5)
	{
		m_pGameObject->RemoveComponent<FPSCounter>();
		m_pGameObject->MarkForDeletion();
	}*/
}
