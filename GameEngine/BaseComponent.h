#pragma once
#include <memory>

namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:
		BaseComponent(GameObject* gameObject) : m_pGameObject{ gameObject } {};
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		void MarkForDeletion() { m_IsMarkedForDeletion = true; };
		bool IsMarkedForDeletion() { return m_IsMarkedForDeletion; };

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		
	protected:
		GameObject* m_pGameObject{}; //baseComponent is not the owner of the gameobject. This could be a replacement for the raw pointer: https://en.cppreference.com/w/cpp/experimental/observer_ptr 

	private:
		bool m_IsMarkedForDeletion{};
	};
}



