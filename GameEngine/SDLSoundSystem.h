#pragma once
#include "SoundSystem.h"
#include <vector>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>

namespace engine
{
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
		class SDLSoundSystemImpl;
		std::unique_ptr<SDLSoundSystemImpl> m_Impl;

		

	};
}
