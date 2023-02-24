#pragma once
#include <memory>

namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		void MarkForDeletion() { m_ToBeDeleted = true; };
		bool IsMarkedForDeletion() { return m_ToBeDeleted; };
		void SetGameObject(GameObject* gameObject) { m_GameObject = gameObject; };

		virtual void Initialize() = 0;

		virtual void Update() = 0;
		
	protected:
		BaseComponent() = default;

		GameObject* m_GameObject{}; //baseComponent is not the owner of the gameobject, this could be a replacement for the raw pointer: https://en.cppreference.com/w/cpp/experimental/observer_ptr 

	private:
		bool m_ToBeDeleted{};
	};
}



