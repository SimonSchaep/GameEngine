#pragma once
#include "PlayerController.h"
#include "glm/glm.hpp"

class MovementComponent;
class PlayerLives;
class PlayerPoints;

class MyPlayerController : public PlayerController
{
public:
    MyPlayerController(GameObject* pGameObject) :PlayerController(pGameObject) {};

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

    GameObject* m_pChef{};
    GameObject* m_pBean{};

    bool m_ControlChef{};

};

