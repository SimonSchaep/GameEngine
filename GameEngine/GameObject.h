#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include "Transform.h"

namespace dae
{
	class BaseComponent;

	class GameObject final
	{
	public:
		void Initialize();
		void Update();
		void Render() const;

		void SetPosition(float x, float y);
		const Transform& GetTransform()const { return m_Transform; };

		void MarkForDeletion() { m_IsMarkedForDeletion = true; };
		bool IsMarkedForDeletion() { return m_IsMarkedForDeletion; };

		//returns the first component of the specified type
		template<typename T>
		T* GetComponent()const;

		//returns all component of the specified type
		template<typename T>
		std::vector<T*> GetAllComponentsOfType()const;

		template<typename T>
		void AddComponent(std::unique_ptr<T> component);

		//removes the first component of the specified type
		template<typename T>
		void RemoveComponent();

		//removes all components of the specified type
		template<typename T>
		void RemoveAllComponentsOfType();

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform{};
		bool m_IsMarkedForDeletion{};

		std::vector<size_t> m_ToDeleteIndexes{};

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
		//no component of specified type was found
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
	void GameObject::AddComponent(std::unique_ptr<T> component)
	{
		m_Components.push_back(std::move(component));
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
