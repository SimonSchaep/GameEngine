#pragma once
#include <memory>

class GameObject;

class BaseComponent
{
public:
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
	BaseComponent(GameObject* gameObject) : m_pGameObject{ gameObject } {};
	GameObject* GetGameObject()const { return m_pGameObject; };
private:
	GameObject* m_pGameObject{}; //baseComponent is not the owner of the gameobject. This could be a replacement for the raw pointer: https://en.cppreference.com/w/cpp/experimental/observer_ptr 

	bool m_IsMarkedForDeletion{};
};



