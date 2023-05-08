#pragma once
#include "RenderComponent.h"
#include <vector>

//todo: finish this

namespace engine
{
	class Collider;

	class ColliderDebugRenderComponent final : public RenderComponent
	{
	public:
		ColliderDebugRenderComponent(GameObject* pGameObject);
		virtual ~ColliderDebugRenderComponent();

		virtual void Initialize()override;
		

		virtual void Render()const override;

	private:
		std::vector<Collider*> m_Colliders{};
	};

}

