#include "Time.h"
#include <chrono>
#include <iostream>

void dae::Time::Update()
{
	static auto startTime{ std::chrono::high_resolution_clock::now() };
	static auto lastTime{ std::chrono::high_resolution_clock::now() };

	const auto currentTime = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
	m_DeltaTime *= m_TimeScale;
	m_TotalTime += m_DeltaTime;
	lastTime = currentTime;

	//Sleep to prevent going over framerate cap
	if (m_FrameRateCap > 0)
	{
		const int msPerFrame = 1'000 / m_FrameRateCap;
		const auto sleepTime = lastTime + std::chrono::milliseconds(msPerFrame) - currentTime;
		std::this_thread::sleep_for(sleepTime);
	}	

	//std::cout << GetTotalTime() << '\n';
	//std::cout << std::chrono::duration<float>(currentTime - startTime).count() << '\n';
}
