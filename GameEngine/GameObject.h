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

		virtual void Update();
		virtual void Render() const;

		//void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		//returns the first component of the specified type
		template<typename T>
		T* GetComponent();

		template<typename T>
		void AddComponent(T* component);

		template<typename T>
		void RemoveComponent(T* component);

		GameObject() = default;
		virtual ~GameObject();
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
	T* GameObject::GetComponent()
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
	void GameObject::AddComponent(T* component)
	{
		m_Components.push_back(std::make_shared<T>(component));
	}
}
