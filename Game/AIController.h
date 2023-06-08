#pragma once
#include <BaseComponent.h>
#include <ObservingPointer.h>
#include <glm/glm.hpp>
#include <memory>
#include "MovementComponent.h"

class MoveCommand;

class AIController : public engine::BaseComponent
{
public:
    AIController(engine::GameObject* pGameObject);

    virtual void Initialize() override;
    virtual void Update() override;

    virtual void SetControlledObject(engine::GameObject* pControlledObject);

protected:
    virtual void ProcessAIDecisions();

    engine::GameObject* GetControlledGameObject()const { return m_pControlledGameObject.Get(); }

    void Move(const glm::vec2& direction);

    void MoveRight() { Move({ 1,0 }); }
    void MoveLeft() { Move({ -1,0 }); }
    void MoveUp() { Move({ 0,1 }); }
    void MoveDown() { Move({ 0,-1 }); }

    bool CanMoveRight() { return m_pControlledMovementComponent->CanMoveRight(); }
    bool CanMoveLeft() { return m_pControlledMovementComponent->CanMoveLeft(); }
    bool CanMoveUp() { return m_pControlledMovementComponent->CanMoveUp(); }
    bool CanMoveDown() { return m_pControlledMovementComponent->CanMoveDown(); }


private:
    engine::ObservingPointer<MovementComponent> m_pControlledMovementComponent;
    engine::ObservingPointer<engine::GameObject> m_pControlledGameObject;
};

