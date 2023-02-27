#include "Time.h"
#include <chrono>
#include <iostream>

void Time::Update()
{
	static auto firstFrameStart{ std::chrono::high_resolution_clock::now() };
	static auto previousFrameStart{ std::chrono::high_resolution_clock::now() };

	//Sleep until enough time has passed to not get more fps than the cap
	if (m_FrameRateCap > 0)
	{
		const int microSecPerFrame = 1'000'000 / m_FrameRateCap;
		const auto timeElapsedSoFar = std::chrono::high_resolution_clock::now() - previousFrameStart;
		const auto sleepTime = std::chrono::microseconds(microSecPerFrame) - timeElapsedSoFar;

		HighResSleep(std::chrono::duration<float>(sleepTime).count());
	}

	//DELTA TIME
	const auto currentFrameStart = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentFrameStart - previousFrameStart).count();
	m_DeltaTime *= m_TimeScale;
	m_TotalTime += m_DeltaTime;

	previousFrameStart = currentFrameStart;
}

//Adapted from:
//https://stackoverflow.com/questions/13397571/precise-thread-sleep-needed-max-1ms-error
void Time::HighResSleep(float sleepTime)
{
	std::chrono::duration<float> MinSleepDuration(0);
	const auto start = std::chrono::high_resolution_clock::now();
	while (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count() < sleepTime)
	{
		std::this_thread::sleep_for(MinSleepDuration);
	}
}
