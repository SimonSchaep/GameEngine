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
		const Transform& GetTransform()const { return m_transform; };

		//returns the first component of the specified type
		template<typename T>
		std::shared_ptr<T> GetComponent()const;

		//returns all component of the specified type
		template<typename T>
		std::vector<std::shared_ptr<T>> GetAllComponentsOfType()const;

		template<typename T>
		void AddComponent(std::shared_ptr<T> component);

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
		Transform m_transform{};

		std::vector<std::shared_ptr<BaseComponent>> m_Components;
		//this could be interesting if getcomponent is too slow:
		//https://stackoverflow.com/questions/9859390/use-data-type-class-type-as-key-in-a-map
	};

	template<typename T>
	std::shared_ptr<T> GameObject::GetComponent()const
	{
		std::shared_ptr<T> returnComponent{};
		for (auto& c : m_Components)
		{
			returnComponent = std::dynamic_pointer_cast<T>(c);
			if (returnComponent)
			{
				return returnComponent;
			}
		}
		//no component of specified type was found
		return nullptr;
	}

	template<typename T>
	std::vector<std::shared_ptr<T>> GameObject::GetAllComponentsOfType()const
	{
		std::vector<std::shared_ptr<T>> returnComponents{};
		
		for (auto& c : m_Components)
		{
			std::shared_ptr<T> component{ std::dynamic_pointer_cast<T>(c) };
			if (component)
			{
				returnComponents.push_back(component);
			}
		}
		return returnComponents;
	}

	template<typename T>
	void GameObject::AddComponent(std::shared_ptr<T> component)
	{
		component->SetGameObject(this);
		m_Components.push_back(std::move(component));		
	}

	template<typename T>
	void GameObject::RemoveComponent()
	{
		for (auto& c : m_Components)
		{
			if (std::dynamic_pointer_cast<T>(c))
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
			if (std::dynamic_pointer_cast<T>(c))
			{
				c->MarkForDeletion();
			}
		}
	}
}
