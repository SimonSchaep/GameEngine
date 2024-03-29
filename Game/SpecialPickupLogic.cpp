#include "SpecialPickupLogic.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "ThrowPepperComponent.h"
#include "GameManager.h"
#include "Scene.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

using namespace engine;

SpecialPickupLogic::SpecialPickupLogic(engine::GameObject* pGameObject, EventType pickupType)
	:BaseComponent(pGameObject)
	, m_PickupType{pickupType}
{
	m_OnPickup = std::make_unique<Event<EventType, SpecialPickupLogic*>>();
}

void SpecialPickupLogic::Initialize()
{
	GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>()->GetOnRespawnCharacters()->AddObserver(this);

	GetGameObject()->GetComponent<BoxCollider>()->GetOnTriggerEvent()->AddObserver(this);

	m_PickupSound = ServiceLocator::GetSoundSystem().AddClip("data/sounds/bonuspoints.wav");
}

void SpecialPickupLogic::Notify(EventType type)
{
	if (type == EventType::respawnCharacters)
	{
		GetGameObject()->MarkForDeletion(true);
	}
}

void SpecialPickupLogic::HandleTriggerEnter(engine::Collider* /*pOriginCollider*/, engine::Collider* pHitCollider)
{
	if (pHitCollider->HasTag("Chef"))
	{
		auto pObject = pHitCollider->GetGameObject();
		auto pPepper = pObject->GetComponent<ThrowPepperComponent>();
		if (pPepper)
		{
			pPepper->AddPepper(1);
			m_OnPickup->NotifyObservers(m_PickupType, this);
			GetGameObject()->MarkForDeletion(true);
		}
		ServiceLocator::GetSoundSystem().Play(m_PickupSound);
	}
}
