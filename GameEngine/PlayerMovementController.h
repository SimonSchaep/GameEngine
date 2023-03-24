#pragma once
#include "PlayerController.h"
#include "MovementComponent.h"
#include "glm/glm.hpp"

class PlayerMovementController : public PlayerController
{
public:
    PlayerMovementController(GameObject* pGameObject) :PlayerController(pGameObject) {};

    virtual void Initialize() override;

    void Move(const glm::vec2& direction);

    void SetControlChef(bool controlChef);
    void SwitchControlledGameObjects();

private:
    MovementComponent* m_pControlledMovementComponent{};

    GameObject* m_pChef{};
    GameObject* m_pBean{};

    bool m_ControlChef{};

};

