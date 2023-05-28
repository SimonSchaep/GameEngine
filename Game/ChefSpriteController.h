#pragma once
#include <BaseComponent.h>
#include "Observer.h"

namespace engine
{
	class SpriteStateMachineComponent;

	class ChefSpriteController : public BaseComponent, Observer<>
	{
	public:
		ChefSpriteController(engine::GameObject* pGameObject);
		virtual ~ChefSpriteController() = default;

		virtual void Initialize() override;
		virtual void Update() override {};

		virtual void Notify()override;

	private:
		SpriteStateMachineComponent* m_pSpriteStateMachine{};

		bool m_HasPlayerWon{};
	};
}