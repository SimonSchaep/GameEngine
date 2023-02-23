#include "RenderComponent.h"
#include "Texture2D.h"
#include "ResourceManager.h"

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}