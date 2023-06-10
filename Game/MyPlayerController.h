#pragma once
#include "PlayerController.h"
#include "ObservingPointer.h"
#include "Observer.h"
#include "EventTypes.h"
#include "glm/glm.hpp"

class MovementComponent;
class PlayerLives;
class PlayerPoints;
class ChefLogic;

class MyPlayerController : public engine::PlayerController, public engine::Observer<EventType>, public engine::Observer<EventType, ChefLogic*>
{
public:
    MyPlayerController(engine::GameObject* pGameObject);

    virtual void Initialize() override;

    virtual void Move(const glm::vec2& direction);

    virtual void SetControlledObject(engine::GameObject* pControlledObject);

    virtual void Notify(EventType type)override;
    virtual void Notify(EventType type, ChefLogic*)override;

    bool IsPaused()const { return m_IsPaused; }

protected:
    MovementComponent* GetMovementComponent()const { return m_pControlledMovementComponent.Get(); }

private:
    engine::ObservingPointer<MovementComponent> m_pControlledMovementComponent;

    bool m_IsPaused{};
};

