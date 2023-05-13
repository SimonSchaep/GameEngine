#pragma once
#include <string>

namespace engine
{
	struct Mix_Chunk;

	class SoundClip
	{
	public:
		virtual ~SoundClip() = default;

		virtual bool IsLoaded() const = 0;
		virtual void Load() = 0;
		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual bool IsPlaying()const = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
		virtual void SetVolume(int value) = 0;
		int GetVolume() const { return m_Volume; }

	protected:
		SoundClip(const std::string& fileName) :m_FileName{fileName} {};

	private:
		std::string m_FileName{};
		int m_ActiveChannel{};
		int m_Volume{};
	};
}