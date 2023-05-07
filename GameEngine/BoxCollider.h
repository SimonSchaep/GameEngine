#pragma once
#include "BaseComponent.h"
#include "Structs.h"
#include "Event.h"

namespace engine
{
	class BoxCollider : public BaseComponent
	{
	public:
		BoxCollider(GameObject* pGameObject);
		Event<>* GetOnCollisionEnterEvent() { return m_OnCollisionEnter.get(); }
		Event<>* GetOnCollisionExitEvent() { return m_OnCollisionExit.get(); }

		bool IsRectInCollider(const structs::Rect& rect);
		bool IsPointInCollider(const glm::vec2& point);
		bool IsCircleInCollider(const structs::Circle& circle);

	private:
		structs::Rect m_Shape{};

		std::unique_ptr<Event<>> m_OnCollisionEnter{};
		std::unique_ptr<Event<>> m_OnCollisionExit{};
	};
}


