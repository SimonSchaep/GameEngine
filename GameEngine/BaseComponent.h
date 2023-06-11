#pragma once
#include "ObservableObject.h"
#include <string>
#include <memory>

namespace engine
{
	class GameObject;
	class Transform;
	class Scene;

	class BaseComponent : public virtual ObservableObject 
		//Has to be virtual inheritance, since otherwise there might be 2 definitions of observableobject in a component that inherits from observer and basecomponent
		//https://www.sandordargo.com/blog/2020/12/23/virtual-inheritance
	{
	public:
		virtual ~BaseComponent();
		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		void MarkForDeletion() { m_IsMarkedForDeletion = true; };
		bool IsMarkedForDeletion() { return m_IsMarkedForDeletion; };

		void InitializeComponent(); //calls Initialize
		void UpdateComponent(); //calls Update
		virtual void Reset() {};
		virtual void OnEnable() {};
		virtual void OnDisable() {};

		GameObject* GetGameObject()const { return m_pGameObject; };
		Transform* GetTransform()const;
		Scene* GetScene()const;
		bool HasTag(const std::string& tag)const;

		void SetIsActive(bool isActive) { m_IsActive = isActive; }
		bool IsActive();

		virtual void OnSceneTransferred() {};

	protected:
		BaseComponent(GameObject* gameObject);

		virtual void Initialize() {};
		virtual void Update() {};
	private:
		GameObject* m_pGameObject{};

		bool m_IsMarkedForDeletion{};

		bool m_IsInitialized{};

		bool m_IsActive{true};
	};
}