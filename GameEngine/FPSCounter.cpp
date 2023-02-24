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
	const std::string text{ "FPS: " + std::to_string(int(glm::round(1.f / Time::GetInstance().GetDeltaTime()))) };
	m_TextRenderComponent->SetText(text);

	//to test removal of components
	/*if (Time::GetInstance().GetTotalTime() > 5)
	{
		m_GameObject->RemoveComponent<FPSCounter>();
	}*/
}
