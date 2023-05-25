#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "Transform.h"
#include "BaseComponent.h"
//#include "Collider.h"

namespace engine
{
	class RenderComponent;
	class UIRenderComponent;
	class Collider;
	class Scene;

	class GameObject final
	{
	public:
		void Initialize();
		void Update();
		void Render()const;
		void RenderUI();

		Transform* GetTransform() { return m_Transform.get(); }

		Scene* GetScene()const;
		const std::string& GetName()const { return m_Name; }
		bool HasTag(const std::string& tag) { return std::find(m_Tags.begin(), m_Tags.end(), tag) != m_Tags.end(); }
		void AddTag(const std::string& tag) { m_Tags.push_back(tag); }

		void MarkAsSceneIndependant();
		bool IsSceneIndependant() { return m_IsSceneIndependant; }

		void MarkForDeletion(bool includeChildren);
		bool IsMarkedForDeletion() { return m_IsMarkedForDeletion; }

		void SetIsActive(bool isActive) { m_IsActive = isActive; }
		bool IsActive() { return m_IsActive && (!m_pParent || m_pParent->IsActive()); } //if all in hierarchy above is active, return true. Here it would be easier if a gameobject owns its children

		//PARENT/CHILDREN
		void SetParent(GameObject* pParent, bool keepWorldPosition);
		GameObject* GetParent()const { return m_pParent; }

		const std::vector<GameObject*>& GetChildren()const { return m_Children; }

		void RegisterCollider(Collider* pCollider) { m_Colliders.push_back(pCollider); }
		const std::vector<Collider*>& GetColliders()const { return m_Colliders; } //to make collisions not have to use GetAllComponentsOfType

		void RegisterRenderComponent(RenderComponent* pRenderComponent) { m_RenderComponents.push_back(pRenderComponent); }
		const std::vector<RenderComponent*>& GetRenderComponents()const { return m_RenderComponents; } //to make rendering not have to use GetAllComponentsOfType

		void RegisterUIRenderComponent(UIRenderComponent* pUIRenderComponent) { m_UIRenderComponents.push_back(pUIRenderComponent); }
		const std::vector<UIRenderComponent*>& GetUIRenderComponents()const { return m_UIRenderComponents; } //to make ui rendering not have to use GetAllComponentsOfType

		//COMPONENTS
		//returns the first m_pComponent of the specified type
		template<typename T>
		T* GetComponent()const;

		//returns all m_pComponent of the specified type
		template<typename T>
		std::vector<T*> GetAllComponentsOfType()const;

		template<typename T>
		T* CreateAndAddComponent();

		//removes the first m_pComponent of the specified type
		template<typename T>
		void RemoveComponent();

		//removes all components of the specified type
		template<typename T>
		void RemoveAllComponentsOfType();

		//ideally this would be private, but that complicates things since the constructor needs to be used in make_shared
		//https://stackoverflow.com/questions/8147027/how-do-i-call-stdmake-shared-on-a-class-with-only-protected-or-private-const/73236821#73236821
		GameObject(const std::string& name, Scene* pScene); //only use this with scene.CreateAndAddGameObject()
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void AddChild(GameObject* pGameObject);
		void RemoveChild(GameObject* pGameObject);

		void SetHierarchyIsSceneIndependant(bool isDependant);

		bool m_IsInitialized{};

		bool m_IsActive{true};

		std::unique_ptr<Transform> m_Transform{};

		Scene* m_pScene{};
		std::string m_Name{};
		std::vector<std::string> m_Tags{};

		bool m_IsSceneIndependant{};

		bool m_IsMarkedForDeletion{};

		std::vector<size_t> m_ToDeleteIndexes{};

		std::vector<GameObject*> m_Children;
		GameObject* m_pParent{};

		std::vector<Collider*> m_Colliders{};
		std::vector<RenderComponent*> m_RenderComponents{};
		std::vector<UIRenderComponent*> m_UIRenderComponents{};

		//todo: maybe use a vector of active and inactive components
		std::vector<std::unique_ptr<BaseComponent>> m_Components{};
		//this could be interesting if getcomponent is too slow:
		//https://stackoverflow.com/questions/9859390/use-data-type-class-type-as-key-in-a-map
	};

	template<typename T>
	T* GameObject::GetComponent()const
	{
		T* returnComponent{};
		for (auto& c : m_Components)
		{
			returnComponent = dynamic_cast<T*>(c.get());
			if (returnComponent)
			{
				return returnComponent;
			}
		}
		//no Component of specified type was found
		//todo: add log
		return nullptr;
	}

	template<typename T>
	std::vector<T*> GameObject::GetAllComponentsOfType()const
	{
		std::vector<T*> returnComponents{};

		for (auto& c : m_Components)
		{
			T* component{ dynamic_cast<T*>(c.get()) };
			if (component)
			{
				returnComponents.push_back(component);
			}
		}
		return returnComponents;
	}

	template<typename T>
	T* GameObject::CreateAndAddComponent()
	{
		auto component = std::make_unique<T>(this);
		T* pReturnValue = component.get();

		m_Components.push_back(std::move(component));
		return pReturnValue;
	}

	template<typename T>
	void GameObject::RemoveComponent()
	{
		for (auto& c : m_Components)
		{
			if (dynamic_cast<T*>(c.get()))
			{
				c->MarkForDeletion();
				return;
			}
		}
	}

	template<typename T>
	void GameObject::RemoveAllComponentsOfType()
	{
		for (auto& c : m_Components)
		{
			if (dynamic_cast<T*>(c.get()))
			{
				c->MarkForDeletion();
			}
		}
	}

}