#pragma once
#include "BaseComponent.h"

namespace engine
{

	class UIRenderComponent : public BaseComponent
	{
	public:
		UIRenderComponent(GameObject* pGameObject);
		virtual ~UIRenderComponent() = default;

		virtual void Initialize() override {};

		virtual void Update() override {};

		virtual void RenderUI() {};
	};

}