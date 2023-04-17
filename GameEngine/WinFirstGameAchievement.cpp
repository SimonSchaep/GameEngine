#include "WinFirstGameAchievement.h"
#include "GameObject.h"
#include "PlayerPoints.h"
#include "CustomEvents.h"

bool WinFirstGameAchievement::UnlockConditionMet(Event*, GameObject* pGameObject)
{
    return pGameObject->GetComponent<PlayerPoints>()->GetPoints() >= m_PointsTreshold;
}

void WinFirstGameAchievement::AddSubjectGameObject(GameObject* pGameObject)
{
    PlayerPoints* pPlayerPoints = pGameObject->GetComponent<PlayerPoints>();

    assert(pPlayerPoints);

    pPlayerPoints->GetKilledEnemyEvent()->AddObserver(this);
}
