#pragma once
#include "MyPlayerController.h"
#include "ObservingPointer.h"
#include "Observer.h"
#include "EventTypes.h"
#include <glm/glm.hpp>

class ThrowPepperComponent;
class ChefLogic;

class ChefPlayerController final : public MyPlayerController
{
public:
    ChefPlayerController(engine::GameObject* pGameObject);

    virtual void Initialize() override;
    virtual void Update() override;

    virtual void Move(const glm::vec2& direction) override;

    virtual void SetControlledObject(engine::GameObject* pControlledObject)override;

    virtual void Notify(EventType type)override;

    void ThrowPepper();

private:
    engine::ObservingPointer<ThrowPepperComponent> m_pThrowPepperComponent{};

    engine::ObservingPointer<ChefLogic> m_pChefLogic{};

    glm::vec2 m_LastMovementDirection{0,-1}; //for throwing while idle

    bool m_HasWon{};
};

