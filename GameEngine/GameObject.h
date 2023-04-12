#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "Transform.h"

class BaseComponent;

class GameObject final
{
public:
	void Initialize();
	void Update();
	void Render()const;
	void RenderUI();

	Transform* GetTransform() { return m_Transform.get(); };

	const std::string& GetName()const { return m_Name; };

	void MarkForDeletion(bool includeChildren);
	bool IsMarkedForDeletion() { return m_IsMarkedForDeletion; };


	//PARENT/CHILDREN
	void SetParent(GameObject* pParent, bool keepWorldPosition);
	GameObject* GetParent()const { return m_Parent; };

	const std::vector<GameObject*>& GetChildren()const { return m_Children; };


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

	GameObject(const std::string& name); //only use this with scene.CreateAndAddGameObject()
	~GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

private:
	void AddChild(GameObject* pGameObject) { m_Children.push_back(pGameObject); };
	void RemoveChild(GameObject* pGameObject) { m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), pGameObject)); };

	std::unique_ptr<Transform> m_Transform{};

	std::string m_Name{};

	bool m_IsMarkedForDeletion{};

	std::vector<size_t> m_ToDeleteIndexes{};

	std::vector<GameObject*> m_Children;
	GameObject* m_Parent{};

	std::vector<std::unique_ptr<BaseComponent>> m_Components;
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