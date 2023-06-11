#pragma once
#include "RenderComponent.h"
#include "ObservingPointer.h"
#include <vector>

//use if you want to draw the colliders of a specific gameobject
//for debug drawing all collisions, uncomment the code in Renderer

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
		std::vector<ObservingPointer<Collider>> m_Colliders{};
	};

}

