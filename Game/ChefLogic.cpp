#include "ChefLogic.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "Scene.h"
#include "GameManager.h"
#include "EnemyLogic.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

using namespace engine;

ChefLogic::ChefLogic(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	m_OnDeath = std::make_unique<Event<EventType, ChefLogic*>>();
}

void ChefLogic::Initialize()
{
	m_Startpos = GetTransform()->GetWorldPosition();

	GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>()->GetOnRespawnCharacters()->AddObserver(this);

	GetGameObject()->GetComponent<BoxCollider>()->GetOnTriggerEvent()->AddObserver(this);

	m_DeathSound = ServiceLocator::GetSoundSystem().AddClip("data/sounds/death.wav");
}

void ChefLogic::Notify(EventType)
{
	Respawn();
}

void ChefLogic::Respawn()
{
	GetTransform()->SetWorldPosition(m_Startpos);
	m_IsDead = false;
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
			ServiceLocator::GetSoundSystem().Play(m_DeathSound);
			m_OnDeath->NotifyObservers(EventType::chefDied, this);
		}
	}
}
