#pragma once
#include "RenderComponent.h"
#include <memory>
#include <glm/glm.hpp>

namespace engine
{
	class Sprite;

	class SpriteRenderComponent : public RenderComponent
	{
	public:
		SpriteRenderComponent(GameObject* pGameObject);
		virtual ~SpriteRenderComponent() = default;

		virtual void Update() override;
		virtual void Render()const override;

		void SetSprite(std::shared_ptr<Sprite> sprite);

		void SetAnimationSpeed(float speed) { m_AnimationSpeed = speed; }

		void Pause();
		void Resume();

		void SetUpdateWhenTimePaused(bool updateWhenTimePaused);

		void SetSize(const glm::vec2& size) { m_Size = size; }
		glm::vec2 GetSize();

		bool GetIsPaused();

	private:
		std::shared_ptr<Sprite> m_Sprite{};

		glm::vec2 m_Size{};

		float m_AnimationSpeed{1.f};

		bool m_UpdateWhenTimePaused{};

		bool m_IsPaused{};
	};
}