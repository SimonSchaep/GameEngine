#include "MoveComponent.h"
#include "GameObject.h"
#include "Time.h"


void MoveComponent::Initialize()
{
}

void MoveComponent::Update()
{
	const float ms{ 10 };
	const float dt{ Time::GetInstance().GetDeltaTime() };

	glm::vec3 pos{ GetGameObject()->GetLocalPosition() };
	pos.x += ms * dt;
	GetGameObject()->SetLocalPosition(pos);
}
