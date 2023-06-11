#pragma once
#include "BaseComponent.h"

namespace engine
{

	class UIRenderComponent : public BaseComponent
	{
	public:
		UIRenderComponent(GameObject* pGameObject);
		virtual ~UIRenderComponent();

		virtual void Initialize() override {};

		virtual void Update() override {};

		virtual void RenderUI() {};

		void SetLayer(int layer) { m_Layer = layer; };
		int GetLayer()const { return m_Layer; };

	private:
		int m_Layer{};
	};

}