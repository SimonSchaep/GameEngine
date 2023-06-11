#include "EnemyLogic.h"
#include "Scene.h"
#include "GameManager.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "TimeManager.h"
#include "FoodParent.h"

using namespace engine;

EnemyLogic::EnemyLogic(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnDeath = std::make_unique<Event<EventType, EnemyLogic*>>();
}

void EnemyLogic::Initialize()
{
	m_Startpos = GetTransform()->GetWorldPosition();

	GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>()->GetOnRespawnCharacters()->AddObserver(this);

	GetGameObject()->GetComponent<BoxCollider>()->GetOnTriggerEvent()->AddObserver(this);
}

void EnemyLogic::Update()
{
	if (m_StunnedDurationTimer > 0 && !IsFalling())
	{
		m_StunnedDurationTimer -= TimeManager::GetInstance().GetDeltaTime();
		if (m_StunnedDurationTimer <= 0)
		{
			m_IsStunned = false;
		}
	}
	if (m_RespawnDelayTimer > 0)
	{
		m_RespawnDelayTimer -= TimeManager::GetInstance().GetDeltaTime();
		if (m_RespawnDelayTimer <= 0)
		{
			Respawn();
		}
	}
}

void EnemyLogic::Respawn()
{
	GetTransform()->SetWorldPosition(m_Startpos);
	GetGameObject()->SetParent(nullptr, true);
	m_IsFalling = false;
	m_IsDead = false;
	m_IsStunned = false;

	if (m_Food)
	{
		//cleanup previous food
		m_Food->GetFallEvent()->RemoveObserver(this);
		m_Food->DecreaseFallExtraLevel();
		m_Food = nullptr;
	}
}

void EnemyLogic::Notify(EventType type)
{
	if (type == EventType::respawnCharacters)
	{
		Respawn();
	}
}

void EnemyLogic::Notify(engine::GameObject* pObject, EventType type)
{
	if (m_IsDead)return;

	if (type == EventType::foodStartFall)
	{
		GetGameObject()->SetParent(pObject, true);
		m_IsFalling = true;
	}
	else if(type == EventType::foodStopFall)
	{
		GetGameObject()->SetParent(nullptr, true);
		m_IsFalling = false;
	}
}

void EnemyLogic::HandleTriggerEnter(engine::Collider* /*pOriginCollider*/, engine::Collider* pHitCollider)
{
	if (pHitCollider->HasTag("Pepper"))
	{
		m_IsStunned = true;
		m_StunnedDurationTimer = m_StunnedDuration;
	}
	if (pHitCollider->HasTag("foodparent"))
	{
		auto pFood = pHitCollider->GetGameObject()->GetComponent<FoodParent>();
		if (!pFood)return;
		if (pFood->IsFalling() && pHitCollider->GetTransform()->GetWorldPosition().y > GetTransform()->GetWorldPosition().y - 8)
		{
			m_IsDead = true;
			m_RespawnDelayTimer = m_RespawnDelay;
			m_OnDeath->NotifyObservers(EventType::enemyDied, this);
		}
		else
		{
			if (m_Food)
			{
				//cleanup previous food
				m_Food->GetFallEvent()->RemoveObserver(this);
				m_Food->DecreaseFallExtraLevel();
			}
			m_Food = pFood;
			m_Food->IncreaseFallExtraLevel();
			m_Food->GetFallEvent()->AddObserver(this);
		}
	}
}

void EnemyLogic::HandleTriggerExit(engine::Collider* /*pOriginCollider*/, engine::Collider* pHitCollider)
{
	if (pHitCollider->HasTag("foodparent"))
	{
		if (m_Food)
		{
			auto pObject = pHitCollider->GetGameObject();
			auto pFood = pObject->GetComponent<FoodParent>();
			if (pFood)
			{
				pFood->GetFallEvent()->RemoveObserver(this);
				pFood->DecreaseFallExtraLevel();
				GetGameObject()->SetParent(nullptr, true);
				m_IsFalling = false;
				m_Food = nullptr;
			}
		}		
	}
}