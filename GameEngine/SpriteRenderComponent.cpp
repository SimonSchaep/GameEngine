#include "SpriteRenderComponent.h"
#include "Sprite.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "Texture2D.h"

engine::SpriteRenderComponent::SpriteRenderComponent(GameObject* pGameObject)
	:RenderComponent(pGameObject)
{
}

void engine::SpriteRenderComponent::Update()
{
	if (m_Sprite)
	{
		m_Sprite->Update(m_AnimationSpeed);
	}
}

void engine::SpriteRenderComponent::Render() const
{
	if (m_Sprite)
	{
		const auto& pos = GetGameObject()->GetTransform()->GetWorldPosition();
		structs::Rect dstRect{};
		dstRect.bottomLeft = pos;
		dstRect.width = m_Size.x;
		dstRect.height = m_Size.y;
		m_Sprite->Render(dstRect);
	}

}

void engine::SpriteRenderComponent::SetSprite(std::shared_ptr<Sprite> sprite)
{
	m_Sprite = sprite;
	m_Sprite->Reset();
	m_Sprite->SetIsPaused(m_IsPaused);
	m_Sprite->SetUpdateWhenTimePaused(m_UpdateWhenTimePaused);
}

void engine::SpriteRenderComponent::Pause()
{
	m_IsPaused = true;
	if (m_Sprite)m_Sprite->SetIsPaused(true);
}

void engine::SpriteRenderComponent::Resume()
{
	m_IsPaused = false;
	if (m_Sprite)m_Sprite->SetIsPaused(false);
}

void engine::SpriteRenderComponent::SetUpdateWhenTimePaused(bool updateWhenTimePaused)
{
	m_UpdateWhenTimePaused = updateWhenTimePaused;
	m_Sprite->SetUpdateWhenTimePaused(updateWhenTimePaused);
}

glm::vec2 engine::SpriteRenderComponent::GetSize()
{
	if (m_Size != glm::vec2{ 0,0 }) return m_Size;
	else return m_Sprite->GetTexture()->GetSize();
}

bool engine::SpriteRenderComponent::GetIsPaused()
{
	return m_Sprite && m_Sprite->GetIsPaused();
}
