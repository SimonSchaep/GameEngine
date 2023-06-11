#pragma once
#include "BaseComponent.h"
#include "ObservingPointer.h"
#include "MovementComponent.h"
#include "Observer.h"
#include "EventTypes.h"
#include <glm/glm.hpp>
#include <memory>

class MoveCommand;
class ChefLogic;

class AIController : public engine::BaseComponent, public engine::Observer<EventType>, public engine::Observer<EventType, ChefLogic*>
{
public:
    AIController(engine::GameObject* pGameObject);

    virtual void Initialize() override;
    virtual void Update() override;

    virtual void SetControlledObject(engine::GameObject* pControlledObject);

    virtual void Notify(EventType type)override;
    virtual void Notify(EventType type, ChefLogic*)override;

protected:
    virtual void ProcessAIDecisions();

    engine::GameObject* GetControlledGameObject()const { return m_pControlledGameObject.Get(); }

    void Move(const glm::vec2& direction);

    void MoveRight() { Move({ 1,0 }); }
    void MoveLeft() { Move({ -1,0 }); }
    void MoveUp() { Move({ 0,1 }); }
    void MoveDown() { Move({ 0,-1 }); }

    bool CanMoveRight()const { return m_pControlledMovementComponent->CanMoveRight(); }
    bool CanMoveLeft()const { return m_pControlledMovementComponent->CanMoveLeft(); }
    bool CanMoveUp()const { return m_pControlledMovementComponent->CanMoveUp(); }
    bool CanMoveDown()const { return m_pControlledMovementComponent->CanMoveDown(); }

    bool IsPaused()const { return m_IsPaused; }


private:
    engine::ObservingPointer<MovementComponent> m_pControlledMovementComponent;
    engine::ObservingPointer<engine::GameObject> m_pControlledGameObject;

    bool m_IsPaused{};
};

