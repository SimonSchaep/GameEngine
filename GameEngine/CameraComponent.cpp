#include "CameraComponent.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include <SDL_opengl.h>

namespace engine
{
	using namespace structs;

	void CameraComponent::Initialize()
	{
		SetScale(m_Scale); //to set camerabox correctly

		if (m_LevelBoundaries == structs::Rect{})
		{
			ServiceLocator::GetLogger().LogLine("no levelboundaries set", LogType::error);
		}
	}
	void CameraComponent::Update()
	{
		glm::vec2 targetPos{ Track() };
		const float accelerationSpeedModifier{ 0.005f };
		if (m_CameraBox.bottomLeft.x - targetPos.x > m_DeadZoneWidth)
		{
			m_Velocity.x = -(m_CameraBox.bottomLeft.x - targetPos.x - m_DeadZoneWidth) * m_Speed * accelerationSpeedModifier;
			if (m_Velocity.x > m_Speed)
			{
				m_Velocity.x = m_Speed;
			}
		}
		else if (m_CameraBox.bottomLeft.x - targetPos.x < -m_DeadZoneWidth)
		{
			m_Velocity.x = -(m_CameraBox.bottomLeft.x - targetPos.x + m_DeadZoneWidth) * m_Speed * accelerationSpeedModifier;
			if (m_Velocity.x < -m_Speed)
			{
				m_Velocity.x = -m_Speed;
			}
		}
		else
		{
			m_Velocity.x = 0;
		}
		if (m_CameraBox.bottomLeft.y - targetPos.y > m_DeadZoneHeight)
		{
			m_Velocity.y = -(m_CameraBox.bottomLeft.y - targetPos.y - m_DeadZoneHeight) * m_Speed * accelerationSpeedModifier;

			if (m_Velocity.y > m_Speed)
			{
				m_Velocity.y = m_Speed;
			}
		}
		else if (m_CameraBox.bottomLeft.y - targetPos.y < -m_DeadZoneHeight)
		{
			m_Velocity.y = -(m_CameraBox.bottomLeft.y - targetPos.y + m_DeadZoneHeight) * m_Speed * accelerationSpeedModifier;

			if (m_Velocity.y < -m_Speed)
			{
				m_Velocity.y = -m_Speed;
			}
		}
		else
		{
			m_Velocity.y = 0;
		}

		m_CameraBox.bottomLeft += m_Velocity * TimeManager::GetInstance().GetDeltaTime();

		ClampToLevelBoundaries();
	}

	void CameraComponent::SetScale(float scale)
	{
		m_Scale = scale;
		auto windowSize{ Renderer::GetInstance().GetWindowSize() };
		m_CameraBox.width = windowSize.x / scale;
		m_CameraBox.height = windowSize.y / scale;
	}

	void CameraComponent::Transform()const
	{
		auto windowSize{ Renderer::GetInstance().GetWindowSize() };
		glScalef(m_Scale/1, m_Scale/1, 1);
		glTranslatef(-m_CameraBox.bottomLeft.x, (-windowSize.y / m_Scale) + m_CameraBox.bottomLeft.y, 0);

#ifdef _DEBUG
		auto shape = m_CameraBox;
		SDL_Rect rect{};
		rect.x = int(shape.bottomLeft.x);
		rect.y = int(windowSize.y) - int(shape.bottomLeft.y);
		rect.w = int(shape.width);
		rect.h = int(shape.height);
		rect.y -= rect.h;

		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
#endif // _DEBUG

	}

	void CameraComponent::SetPosition(const glm::vec2& pos)
	{
		m_CameraBox.bottomLeft.x = pos.x - m_CameraBox.width / 2;
		m_CameraBox.bottomLeft.y = pos.y - m_CameraBox.height / 2;
	}

	void CameraComponent::SetLevelBoundaries(const Rect& levelBoundaries)
	{
		m_LevelBoundaries = levelBoundaries;
	}

	glm::vec2 CameraComponent::GetCameraPosition()const
	{
		return m_CameraBox.bottomLeft;
	}

	const Rect& CameraComponent::GetCameraBox() const
	{
		return m_CameraBox;
	}

	float CameraComponent::GetCameraSpeed() const
	{
		return m_Speed;
	}

	glm::vec2 CameraComponent::Track()const
	{
		//for rect:
		//return glm::vec2{ target.left + target.width / 2 - m_CameraBox.width / 2 , target.bottom + target.height / 2 - m_CameraBox.height / 2 };

		auto gameObjectPosition = GetGameObject()->GetTransform()->GetWorldPosition();
		return glm::vec2{ gameObjectPosition.x - m_CameraBox.width / 2 , gameObjectPosition.y - m_CameraBox.height / 2};
	}

	void CameraComponent::ClampToLevelBoundaries()
	{
		if (m_CameraBox.bottomLeft.x < m_LevelBoundaries.bottomLeft.x)
		{
			m_CameraBox.bottomLeft.x = m_LevelBoundaries.bottomLeft.x;
		}
		if (m_CameraBox.bottomLeft.x + m_CameraBox.width > m_LevelBoundaries.bottomLeft.x + m_LevelBoundaries.width)
		{
			m_CameraBox.bottomLeft.x = m_LevelBoundaries.bottomLeft.x + m_LevelBoundaries.width - m_CameraBox.width;
		}
		if (m_CameraBox.bottomLeft.y < m_LevelBoundaries.bottomLeft.y)
		{
			m_CameraBox.bottomLeft.y = m_LevelBoundaries.bottomLeft.y;
		}
		if (m_CameraBox.bottomLeft.y + m_CameraBox.height > m_LevelBoundaries.bottomLeft.y + m_LevelBoundaries.height)
		{
			m_CameraBox.bottomLeft.y = m_LevelBoundaries.bottomLeft.y + m_LevelBoundaries.height - m_CameraBox.height;
		}
	}
}