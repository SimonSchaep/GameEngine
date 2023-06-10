#pragma once
#include "MyPlayerController.h"
#include "ObservingPointer.h"
#include <glm/glm.hpp>
#include "Observer.h"
#include "EventTypes.h"

class ThrowPepperComponent;
class ChefLogic;

class ChefPlayerController : public MyPlayerController, engine::Observer<EventType>
{
public:
    ChefPlayerController(engine::GameObject* pGameObject);

    virtual void Initialize() override;
    virtual void Update() override;

    virtual void Move(const glm::vec2& direction) override;

    virtual void SetControlledObject(engine::GameObject* pControlledObject)override;

    void ThrowPepper();

    virtual void Notify(EventType)override;

private:
    engine::ObservingPointer<ThrowPepperComponent> m_pThrowPepperComponent{};

    engine::ObservingPointer<ChefLogic> m_pChefLogic{};

    bool m_IsDead{};

    glm::vec2 m_LastMovementDirection{0,-1}; //for throwing while idle
};

