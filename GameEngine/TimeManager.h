#pragma once
#include "Singleton.h"

namespace engine
{

	//Class for all time-related logic
	class TimeManager final : public Singleton<TimeManager>
	{
	public:
		void Update();

		void SetFixedTimeStep(float fixedTimeStep) { m_FixedTimeStep = fixedTimeStep; };
		float GetFixedTimeStep()const { return m_FixedTimeStep; };

		void SetFrameRateCap(int frameRateCap) { m_FrameRateCap = frameRateCap; }; //-1 for no cap
		int GetFrameRateCap()const { return m_FrameRateCap; }; //-1 for no cap

		void SetTimeScale(float timeScale) { m_TimeScale = timeScale; };
		float GetTimeScale()const { return m_TimeScale; };

		float GetDeltaTime()const { return m_DeltaTime; };
		float GetTotalTime()const { return m_TotalTime; };
	private:
		friend class Singleton<TimeManager>;
		TimeManager() = default;

		void HighResSleep(float sleepTime);

		float m_DeltaTime{};
		float m_TotalTime{};
		float m_FixedTimeStep{ 1 / 60.f };
		float m_TimeScale{ 1 };
		int m_FrameRateCap{ 240 }; //-1 for no cap
	};

}