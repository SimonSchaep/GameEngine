#pragma once
#include <memory>
#include "ObservableObject.h"

namespace engine
{
	class GameObject;
	class Transform;

	class BaseComponent : public ObservableObject
	{
	public:
		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		void MarkForDeletion() { m_IsMarkedForDeletion = true; };
		bool IsMarkedForDeletion() { return m_IsMarkedForDeletion; };

		void InitializeComponent(); //calls Initialize
		void UpdateComponent(); //calls Update

		GameObject* GetGameObject()const { return m_pGameObject; };
		Transform* GetTransform()const;

		void SetIsActive(bool isActive) { m_IsActive = isActive; }
		bool IsActive() { return m_IsActive; }

		virtual void OnSceneTransferred() {};

	protected:
		BaseComponent(GameObject* gameObject);

		virtual void Initialize() = 0;
		virtual void Update() = 0;
	private:
		GameObject* m_pGameObject{}; //baseComponent is not the owner of the gameobject. This could be a replacement for the raw pointer: https://en.cppreference.com/w/cpp/experimental/observer_ptr 

		bool m_IsMarkedForDeletion{};

		bool m_IsInitialized{};

		bool m_IsActive{true};
	};
}