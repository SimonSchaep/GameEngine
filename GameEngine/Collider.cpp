#include "Collider.h"
#include "GameObject.h"

engine::Collider::Collider(GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnTrigger = std::make_unique<Event<TriggerType, Collider*, Collider*>>();

	m_OnDestroy = std::make_unique<Event<Collider*>>();

	pGameObject->RegisterCollider(this);
}

engine::Collider::~Collider()
{
	GetGameObject()->RemoveCollider(this);
	m_OnDestroy->NotifyObservers(this);
}

void engine::Collider::OnDisable()
{
	for (auto& trigger : m_CurrentTriggers)
	{
		trigger->GetOnDestroyEvent()->RemoveObserver(this);
	}
	m_CurrentTriggers.clear();
}

void engine::Collider::AddCurrentTrigger(Collider* pCollider)
{
	m_CurrentTriggers.emplace_back(pCollider);
	pCollider->GetOnDestroyEvent()->AddObserver(this);
}

void engine::Collider::RemoveCurrentTrigger(Collider* pCollider)
{
	m_CurrentTriggers.erase(std::remove(m_CurrentTriggers.begin(), m_CurrentTriggers.end(), pCollider));
	pCollider->GetOnDestroyEvent()->RemoveObserver(this);
}

void engine::Collider::Notify(Collider* pCollider)
{
	auto it = std::find(m_CurrentTriggers.begin(), m_CurrentTriggers.end(), pCollider);
	if (it != m_CurrentTriggers.end())
	{
		m_CurrentTriggers.erase(it);
		m_OnTrigger->NotifyObservers(TriggerType::exit, this, pCollider);
	}
}