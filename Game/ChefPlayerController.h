#pragma once
#include "MyPlayerController.h"
#include "ObservingPointer.h"
#include <glm/glm.hpp>

class ThrowPepperComponent;

class ChefPlayerController : public MyPlayerController
{
public:
    ChefPlayerController(engine::GameObject* pGameObject);

    virtual void Initialize() override;
    virtual void Update() override;

    virtual void SetControlledObject(engine::GameObject* pControlledObject)override;

    void ThrowPepper();

private:
    engine::ObservingPointer<ThrowPepperComponent> m_pThrowPepperComponent{};

    glm::vec2 m_LastMovementDirection{0,-1};
};

