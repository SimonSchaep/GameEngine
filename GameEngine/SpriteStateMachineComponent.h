#pragma once
#include "BaseComponent.h"
#include "ObservingPointer.h"
#include <vector>
#include <memory>
#include <functional>

namespace engine
{
	class SpriteState;
	class SpriteRenderComponent;
	class Sprite;

	class SpriteStateMachineComponent final : public BaseComponent
	{
	public:
		SpriteStateMachineComponent(GameObject* pGameObject);
		virtual ~SpriteStateMachineComponent();

		virtual void Initialize() override;
		virtual void Update() override;

		void ForceStateEvaluation();

		SpriteState* CreateAndAddState(std::shared_ptr<Sprite> sprite);
		void SetStartState(SpriteState* state) { m_CurrentState = state; }
		void SetSpriteRenderComponent(SpriteRenderComponent* pSpriteRenderComponent);

	private:
		void EvaluateStates();

		std::vector<std::unique_ptr<SpriteState>> m_SpriteStates{};

		SpriteState* m_CurrentState{};
		ObservingPointer<SpriteRenderComponent> m_pSpriteRenderComponent{};
	};
}