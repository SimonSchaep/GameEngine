#pragma once
#include "SoundSystem.h"
#include <vector>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>

namespace engine
{
	class SDLSoundClip;

	enum class SoundAction
	{
		play,
		stop,
		pause,
		resume,
	};

	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		virtual ~SDLSoundSystem();
				
		virtual int AddClip(const std::string& fileName) override;
		virtual void Play(int clipId)override;
		virtual void Stop(int clipId)override;
		virtual void Pause(int clipId)override;
		virtual void Resume(int clipId)override;

		virtual void StopAll()override;

	private:
		void ProcessSounds(); //thread-safe

		std::vector<std::unique_ptr<SDLSoundClip>> m_SoundClips;

		std::vector<std::pair<SDLSoundClip*, SoundAction>> m_SoundsToProcess{};

		int m_ChannelCount{8};

		std::jthread m_SoundThread{};
		std::condition_variable m_DoesQueueNeedProcessing{};
		std::mutex m_SoundsToProcessMutex{};

		bool m_EndQueue{};

	};
}
