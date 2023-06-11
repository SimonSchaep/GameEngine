#pragma once
#include <string>

namespace engine
{
	class SoundClip;

	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;

		virtual int AddClip(const std::string& fileName) = 0; //returns clipId, -1 if not valid
		virtual void Play(int clipId) = 0;
		virtual void Stop(int clipId) = 0;
		virtual void Pause(int clipId) = 0;
		virtual void Resume(int clipId) = 0;
		virtual void SetVolume(int clipId, int volume) = 0;

		virtual void Mute() = 0;
		virtual void UnMute() = 0;
		virtual void SetGlobalVolume(int volume) = 0;
		virtual void StopAll() = 0;

	protected:
		SoundSystem() = default;
	};
}