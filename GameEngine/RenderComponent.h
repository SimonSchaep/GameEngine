#pragma once
#include "BaseComponent.h"

namespace engine
{

	class RenderComponent : public BaseComponent
	{
	public:
		RenderComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
		virtual ~RenderComponent() = default;

		virtual void Initialize() override {};

		virtual void Update() override {};

		virtual void Render()const {};

		void SetLayer(int layer) { m_Layer = layer; };
		int GetLayer()const { return m_Layer; };

	private:
		int m_Layer{};
	};

}