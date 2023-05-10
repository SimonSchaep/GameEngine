#include "CameraComponent.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "Transform.h"
#include <iostream>
#include <SDL_opengl.h>

//todo: check the code of this, refactor if necessary

namespace engine
{
	void CameraComponent::Update()
	{
		glm::vec2 targetPos{ Track() };
		const float accelerationSpeedModifier{ 0.005f };
		if (m_CameraBox.BottomLeft.x - targetPos.x > m_DeadZoneWidth)
		{
			m_Velocity.x = -(m_CameraBox.BottomLeft.x - targetPos.x - m_DeadZoneWidth) * m_Speed * accelerationSpeedModifier;
			if (m_Velocity.x > m_Speed)
			{
				m_Velocity.x = m_Speed;
			}
		}
		else if (m_CameraBox.BottomLeft.x - targetPos.x < -m_DeadZoneWidth)
		{
			m_Velocity.x = -(m_CameraBox.BottomLeft.x - targetPos.x + m_DeadZoneWidth) * m_Speed * accelerationSpeedModifier;
			if (m_Velocity.x < -m_Speed)
			{
				m_Velocity.x = -m_Speed;
			}
		}
		else
		{
			m_Velocity.x = 0;
		}
		if (m_CameraBox.BottomLeft.y - targetPos.y > m_DeadZoneHeight)
		{
			m_Velocity.y = -(m_CameraBox.BottomLeft.y - targetPos.y - m_DeadZoneHeight) * m_Speed * accelerationSpeedModifier;

			if (m_Velocity.y > m_Speed)
			{
				m_Velocity.y = m_Speed;
			}
		}
		else if (m_CameraBox.BottomLeft.y - targetPos.y < -m_DeadZoneHeight)
		{
			m_Velocity.y = -(m_CameraBox.BottomLeft.y - targetPos.y + m_DeadZoneHeight) * m_Speed * accelerationSpeedModifier;

			if (m_Velocity.y < -m_Speed)
			{
				m_Velocity.y = -m_Speed;
			}
		}
		else
		{
			m_Velocity.y = 0;
		}

		m_CameraBox.BottomLeft += m_Velocity * TimeManager::GetInstance().GetDeltaTime();

		Clamp();
	}

	void CameraComponent::Transform(float parralaxMultiplier)const
	{
		glTranslatef(-m_CameraBox.BottomLeft.x * parralaxMultiplier, -m_CameraBox.BottomLeft.y * parralaxMultiplier, 0);
	}

	void CameraComponent::SetPosition(const glm::vec2& pos)
	{
		m_CameraBox.BottomLeft.x = pos.x - m_CameraBox.Width / 2;
		m_CameraBox.BottomLeft.y = pos.y - m_CameraBox.Height / 2;
	}

	void CameraComponent::SetLevelBoundaries(const Rect& levelBoundaries)
	{
		m_LevelBoundaries = levelBoundaries;
	}

	glm::vec2 CameraComponent::GetCameraPosition()const
	{
		return m_CameraBox.BottomLeft;
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

		return glm::vec2{ gameObjectPosition.x - m_CameraBox.Width / 2 , gameObjectPosition.y - m_CameraBox.Height / 2};
	}

	void CameraComponent::Clamp()
	{
		if (m_CameraBox.BottomLeft.x < m_LevelBoundaries.BottomLeft.x)
		{
			m_CameraBox.BottomLeft.x = m_LevelBoundaries.BottomLeft.x;
		}
		if (m_CameraBox.BottomLeft.x + m_CameraBox.Width > m_LevelBoundaries.BottomLeft.x + m_LevelBoundaries.Width)
		{
			m_CameraBox.BottomLeft.x = m_LevelBoundaries.BottomLeft.x + m_LevelBoundaries.Width - m_CameraBox.Width;
		}
		if (m_CameraBox.BottomLeft.y < m_LevelBoundaries.BottomLeft.y)
		{
			m_CameraBox.BottomLeft.y = m_LevelBoundaries.BottomLeft.y;
		}
		if (m_CameraBox.BottomLeft.y + m_CameraBox.Height > m_LevelBoundaries.BottomLeft.y + m_LevelBoundaries.Height)
		{
			m_CameraBox.BottomLeft.y = m_LevelBoundaries.BottomLeft.y + m_LevelBoundaries.Height - m_CameraBox.Height;
		}
	}
}