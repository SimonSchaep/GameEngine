#pragma once
#include "SoundSystem.h"
#include <memory>

namespace engine
{
	class LoggingSoundSystem : public SoundSystem
	{
	public:
		LoggingSoundSystem();
		virtual ~LoggingSoundSystem() = default;

		template<typename T>
		void SetActualSoundSystem();

		virtual int AddClip(const std::string& fileName) override;
		virtual void Play(int clipId) override;
		virtual void Stop(int clipId) override;
		virtual void Pause(int clipId) override;
		virtual void Resume(int clipId) override;

		virtual void StopAll();

	private:
		std::unique_ptr<SoundSystem> m_ActualSoundSystem{};
	};

	template<typename T>
	void LoggingSoundSystem::SetActualSoundSystem()
	{
		m_ActualSoundSystem = std::make_unique<T>();
	}
}

