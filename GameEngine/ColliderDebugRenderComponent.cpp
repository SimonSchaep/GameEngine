#include "ColliderDebugRenderComponent.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "GameObject.h"
#include "SDL.h"
#include "Renderer.h"
#include <iostream>

engine::ColliderDebugRenderComponent::ColliderDebugRenderComponent(GameObject* pGameObject) :RenderComponent(pGameObject)
{
}

engine::ColliderDebugRenderComponent::~ColliderDebugRenderComponent()
{
}

void engine::ColliderDebugRenderComponent::Initialize()
{
	m_Colliders = GetGameObject()->GetAllComponentsOfType<Collider>();
}

void engine::ColliderDebugRenderComponent::Render() const
{
	//draw colliders
	for (Collider* pCollider : m_Colliders)
	{
		BoxCollider* pBox = dynamic_cast<BoxCollider*>(pCollider);
		if (pBox)
		{
			auto shape = pBox->GetShape();
			shape.bottomLeft += glm::vec2(pBox->GetGameObject()->GetTransform()->GetWorldPosition().x, pBox->GetGameObject()->GetTransform()->GetWorldPosition().y);
			SDL_Rect rect{};
			rect.x = int(shape.bottomLeft.x);
			rect.y = int(Renderer::GetInstance().GetWindowSize().y) - int(shape.bottomLeft.y);
			rect.w = int(shape.width);
			rect.h = int(shape.height);
			rect.y -= rect.h;

			SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
			SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
		}
	}
}
