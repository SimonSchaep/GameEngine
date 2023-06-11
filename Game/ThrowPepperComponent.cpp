#include "ThrowPepperComponent.h"
#include "TimeManager.h"
#include "Scene.h"
#include "LifeTimer.h"
#include "SpriteRenderComponent.h"
#include "ResourceManager.h"
#include "BoxCollider.h"
#include "GamePlayingState.h"
#include "GameManager.h"

using namespace engine;

void ThrowPepperComponent::Initialize()
{
	m_GamePlayingState = GetScene()->FindGameObjectByName("GameManager")->GetComponent<GameManager>()->GetGamePlayingState();
}

void ThrowPepperComponent::Update()
{
	if (m_ThrowDurationTimer > 0)
	{
		m_ThrowDurationTimer -= TimeManager::GetInstance().GetDeltaTime();
	}
	if (m_ThrowCooldownTimer > 0)
	{
		m_ThrowCooldownTimer -= TimeManager::GetInstance().GetDeltaTime();
	}
}

void ThrowPepperComponent::ThrowPepper(const glm::vec2& direction)
{
	if (m_ThrowCooldownTimer <= 0 && m_GamePlayingState->GetPepperLeft() > 0)
	{
		m_ThrowDurationTimer = m_ThrowDuration;
		m_ThrowCooldownTimer = m_ThrowCooldown;

		m_GamePlayingState->AddPepper(-1);

		CreatePepperObject(direction);
	}
}

void ThrowPepperComponent::AddPepper(int amount)
{
	m_GamePlayingState->AddPepper(amount);
}

void ThrowPepperComponent::CreatePepperObject(glm::vec2 direction)
{
	assert(direction != glm::vec2{});

	glm::vec2 pos = GetGameObject()->GetTransform()->GetWorldPosition();

	if (abs(direction.x) > abs(direction.y))
	{
		if (direction.x > 0)
		{
			direction = { 1,0 };
		}
		else
		{
			direction = { -1,0 };
		}
	}
	else
	{
		if (direction.y > 0)
		{
			direction = { 0,1 };
		}
		else
		{
			direction = { 0,-1 };
		}
	}

	pos += direction * m_ThrowDistance;

	auto pepperObject = GetScene()->CreateAndAddGameObject("Pepper");
	pepperObject->AddTag("Pepper");
	pepperObject->GetTransform()->SetWorldPosition(pos);
	pepperObject->CreateAndAddComponent<LifeTimer>(.6f);

	//visuals
	auto pVisuals = GetScene()->CreateAndAddGameObject("PepperVisuals", pepperObject);
	auto spriteRenderer = pVisuals->CreateAndAddComponent<SpriteRenderComponent>();
	spriteRenderer->SetSize({ 32, 32 });

	float width = float(spriteRenderer->GetSize().x);
	float height = float(spriteRenderer->GetSize().y);
	pVisuals->GetTransform()->SetLocalPosition({ -width / 2, -8 });

	if (direction.x == 1)
	{
		spriteRenderer->SetSprite(std::move(ResourceManager::GetInstance().LoadSprite("pepperThrownRight.png", 4, 1, .1f, 0, 3, false, false)));
	}
	else if (direction.x == -1)
	{
		spriteRenderer->SetSprite(std::move(ResourceManager::GetInstance().LoadSprite("pepperThrownRight.png", 4, 1, .1f, 0, 3, false, false, true)));
	}
	else if (direction.y == 1)
	{
		spriteRenderer->SetSprite(std::move(ResourceManager::GetInstance().LoadSprite("pepperThrownUp.png", 4, 1, .1f, 0, 3, false, false)));
	}
	else if (direction.y == -1)
	{
		spriteRenderer->SetSprite(std::move(ResourceManager::GetInstance().LoadSprite("pepperThrownDown.png", 4, 1, .1f, 0, 3, false, false)));
	}

	//collider
	auto collider = pepperObject->CreateAndAddComponent<BoxCollider>();
	collider->SetShape({ -width / 2, -8, width, height });
}
