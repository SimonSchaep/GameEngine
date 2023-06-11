#pragma once
#include "SoundSystem.h"

namespace engine
{
	class NullSoundSystem final : public SoundSystem
	{
	public:
		NullSoundSystem() = default;
		virtual int AddClip(const std::string&) override { return -1; }
		virtual void Play(int, int)override {}
		virtual void Stop(int)override {}
		virtual void Pause(int)override {}
		virtual void Resume(int)override {}
		virtual void SetVolume(int, int)override {}
		virtual bool IsPlaying(int)override { return false; };

		virtual void Mute()override {}
		virtual void UnMute()override {}
		virtual void SetGlobalVolume(int)override {}
		virtual void StopAll()override {}

	};
}