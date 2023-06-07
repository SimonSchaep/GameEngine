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

    void SetControlledObject(engine::GameObject* pControlledObject);

private:
    void Move(const glm::vec2& direction);

    void ProcessAIDecisions();

    void MoveRight() { Move({1,0}); }
    void MoveLeft() { Move({ -1,0 }); }
    void MoveUp() { Move({ 0,1 }); }
    void MoveDown() { Move({ 0,-1 }); }

    engine::ObservingPointer<MovementComponent> m_pControlledMovementComponent;
};

