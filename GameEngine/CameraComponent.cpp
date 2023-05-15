#include "CameraComponent.h"
#include "TimeManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include <iostream>
#include <SDL_opengl.h>

//todo: check the code of this, refactor if necessary

namespace engine
{
	using namespace structs;

	void CameraComponent::Initialize()
	{
		SetScale(m_Scale); //to set camerabox correctly

		//todo: do this, implement ==
		/*if (m_LevelBoundaries == structs::Rect{})
		{
			std::cerr << "no levelboundaries set\n";
		}*/
		if (m_LevelBoundaries.Width == 0)
		{
			std::cerr << "no levelboundaries set\n";
		}
	}
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

		ClampToLevelBoundaries();
	}

	void CameraComponent::SetScale(float scale)
	{
		m_Scale = scale;
		auto windowSize{ Renderer::GetInstance().GetWindowSize() };
		m_CameraBox.Width = windowSize.x / scale;
		m_CameraBox.Height = windowSize.y / scale;
	}

	void CameraComponent::Transform()const
	{
		auto windowSize{ Renderer::GetInstance().GetWindowSize() };
		glScalef(m_Scale/1, m_Scale/1, 1);
		glTranslatef(-m_CameraBox.BottomLeft.x, (-windowSize.y / m_Scale) + m_CameraBox.BottomLeft.y, 0);

#ifdef _DEBUG
		auto shape = m_CameraBox;
		SDL_Rect rect{};
		rect.x = int(shape.BottomLeft.x);
		rect.y = int(windowSize.y) - int(shape.BottomLeft.y);
		rect.w = int(shape.Width);
		rect.h = int(shape.Height);
		rect.y -= rect.h;

		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 255, 255);
		SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
#endif // _DEBUG

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

	void CameraComponent::ClampToLevelBoundaries()
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