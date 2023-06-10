#include "ChefLogic.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "Scene.h"
#include "GameManager.h"
#include "EnemyLogic.h"

using namespace engine;

ChefLogic::ChefLogic(engine::GameObject* pGameObject, int lives)
	:BaseComponent(pGameObject)
	, m_LivesLeft{ lives }
{
	m_OnDeath = std::make_unique<Event<EventType, ChefLogic*>>();
}

void ChefLogic::Initialize()
{
	m_Startpos = GetTransform()->GetWorldPosition();

	GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>()->GetOnRespawnCharacters()->AddObserver(this);

	GetGameObject()->GetComponent<BoxCollider>()->GetOnTriggerEvent()->AddObserver(this);
}

void ChefLogic::Notify(EventType)
{
	Respawn();
}

void ChefLogic::Respawn()
{
	if (m_LivesLeft > 0)
	{
		GetTransform()->SetWorldPosition(m_Startpos);
		m_IsDead = false;
	}
	else
	{
		GetGameObject()->SetIsActive(false);
	}
}

void ChefLogic::HandleTriggerEnter(engine::Collider* /*pOriginCollider*/, engine::Collider* pHitCollider)
{
	if (m_IsDead) return;

	if (pHitCollider->HasTag("Enemy"))
	{
		auto pEnemyLogic = pHitCollider->GetGameObject()->GetComponent<EnemyLogic>();
		if (!pEnemyLogic->IsStunned() && !pEnemyLogic->IsDead() && !pEnemyLogic->IsFalling())
		{
			m_IsDead = true;
			m_LivesLeft--;
			m_OnDeath->NotifyObservers(EventType::chefDied, this);
		}
	}
}
