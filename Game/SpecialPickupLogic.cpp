#include "SpecialPickupLogic.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "ThrowPepperComponent.h"
#include "GameManager.h"
#include "Scene.h"

using namespace engine;

SpecialPickupLogic::SpecialPickupLogic(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
}

void SpecialPickupLogic::Initialize()
{
	GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>()->GetOnRespawnCharacters()->AddObserver(this);

	GetGameObject()->GetComponent<BoxCollider>()->GetOnTriggerEvent()->AddObserver(this);
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
			GetGameObject()->MarkForDeletion(true);
		}
	}
}
