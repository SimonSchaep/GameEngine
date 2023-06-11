#pragma once
#include "MyPlayerController.h"
#include "ObservingPointer.h"
#include "Observer.h"
#include "EventTypes.h"
#include <glm/glm.hpp>

class EnemyLogic;

class EnemyPlayerController final : public MyPlayerController
{
public:
    EnemyPlayerController(engine::GameObject* pGameObject);

    virtual void Move(const glm::vec2& direction) override;

    virtual void SetControlledObject(engine::GameObject* pControlledObject)override;

private:
    engine::ObservingPointer<EnemyLogic> m_pEnemyLogic{};
};

