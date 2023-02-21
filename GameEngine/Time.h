#pragma once
#include "Singleton.h"

namespace dae
{
	class Time final : public Singleton<Time>
	{
	public:
		void Update();

		void SetFixedTimeStep(float fixedTimeStep) { m_FixedTimeStep = fixedTimeStep; };
		float GetFixedTimeStep() { return m_FixedTimeStep; };

		float GetDeltaTime() { return m_DeltaTime; };
		float GetTotalTime() { return m_TotalTime; };
	private:
		float m_DeltaTime{};
		float m_TotalTime{};
		float m_FixedTimeStep{ 1/60.f };
	};
}
