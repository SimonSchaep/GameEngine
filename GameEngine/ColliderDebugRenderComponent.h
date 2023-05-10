#pragma once
#include "RenderComponent.h"
#include <vector>

//might not be necessary anymore since the collisionmanager can now debug draw

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

