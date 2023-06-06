#pragma once
#include "PlayerController.h"
#include "ObservingPointer.h"
#include "glm/glm.hpp"

class MovementComponent;
class PlayerLives;
class PlayerPoints;

class MyPlayerController : public engine::PlayerController
{
public:
    MyPlayerController(engine::GameObject* pGameObject);

    virtual void Initialize() override;

    void Move(const glm::vec2& direction);

    void SetControlledObject(engine::GameObject* pControlledObject);

private:
    engine::ObservingPointer<MovementComponent> m_pControlledMovementComponent;
    engine::ObservingPointer<PlayerLives> m_pControlledPlayerLivesComponent;
    engine::ObservingPointer<PlayerPoints> m_pControlledPlayerPointsComponent;

};

