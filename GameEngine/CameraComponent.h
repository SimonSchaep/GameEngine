#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"
#include "Structs.h"

namespace engine
{
	using namespace structs;

	class CameraComponent final : public BaseComponent
	{
	public:
		CameraComponent(GameObject* pGameObject) :BaseComponent(pGameObject) {};
		virtual ~CameraComponent() = default;

		virtual void Update() override;

		void SetCameraBox(const Rect& cameraBox) { m_CameraBox = cameraBox; }
		void SetSpeed(float speed) { m_Speed = speed; }
		void SetDeadZone(float width, float height) { m_DeadZoneWidth = width; m_DeadZoneHeight = height; };
		void Transform(float parralaxMultiplier = 1)const;
		void SetPosition(const glm::vec2& pos);
		void SetLevelBoundaries(const Rect& levelBoundaries);
		glm::vec2 GetCameraPosition()const;
		const Rect& GetCameraBox()const;
		float GetCameraSpeed()const;

	private:
		glm::vec2 Track()const;
		void Clamp();

		Rect m_CameraBox{0,0,200,200};
		Rect m_LevelBoundaries{};

		float m_Speed{1000};

		glm::vec2 m_Velocity{};

		float m_DeadZoneWidth{20};
		float m_DeadZoneHeight{20};
	};


}

