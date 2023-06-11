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
		virtual void Play(int clipId, int loops = 0) override;
		virtual void Stop(int clipId) override;
		virtual void Pause(int clipId) override;
		virtual void Resume(int clipId) override;
		virtual void SetVolume(int clipId, int volume) override;
		virtual bool IsPlaying(int clipId)override;

		virtual void Mute()override;
		virtual void UnMute()override;
		virtual void SetGlobalVolume(int volume)override;
		virtual void StopAll()override;

	private:
		std::unique_ptr<SoundSystem> m_ActualSoundSystem{};
	};

	template<typename T>
	void LoggingSoundSystem::SetActualSoundSystem()
	{
		m_ActualSoundSystem = std::make_unique<T>();
	}
}

