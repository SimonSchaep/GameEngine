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

    virtual void SetControlledObject(engine::GameObject* pControlledObject);

protected:
    MovementComponent* GetMovementComponent()const { return m_pControlledMovementComponent.Get(); }

private:
    engine::ObservingPointer<MovementComponent> m_pControlledMovementComponent;

};

