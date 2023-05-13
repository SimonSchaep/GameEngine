#pragma once
#include "PlayerController.h"
#include "glm/glm.hpp"

class MovementComponent;
class PlayerLives;
class PlayerPoints;

using namespace engine;

class MyPlayerController : public engine::PlayerController
{
public:
    MyPlayerController(engine::GameObject* pGameObject) :PlayerController(pGameObject) {};

    virtual void Initialize() override;

    void Move(const glm::vec2& direction);
    void Die();
    void ResetLives();
    void KilledEnemy();

    void SetControlChef(bool controlChef);
    void SwitchControlledGameObjects();

private:
    MovementComponent* m_pControlledMovementComponent{};
    PlayerLives* m_pControlledPlayerLivesComponent{};
    PlayerPoints* m_pControlledPlayerPointsComponent{};

    engine::GameObject* m_pChef{};
    engine::GameObject* m_pBean{};

    bool m_ControlChef{};

};

