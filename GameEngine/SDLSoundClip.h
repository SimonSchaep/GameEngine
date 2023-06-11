#pragma once
#include <string>

struct Mix_Chunk;

namespace engine
{
	class SDLSoundClip final
	{
	public:
		SDLSoundClip(const std::string& fileName);
		~SDLSoundClip();
		SDLSoundClip(const SDLSoundClip& other) = delete;
		SDLSoundClip& operator=(const SDLSoundClip& rhs) = delete;
		SDLSoundClip(SDLSoundClip&& other) = delete;
		SDLSoundClip& operator=(SDLSoundClip&& rhs) = delete;

		bool IsLoaded() const;
		void Load();
		void Play(int loops);
		void Stop();
		bool IsPlaying()const;
		void Pause();
		void Resume();
		void SetVolume(int volume);
		int GetVolume()const;

	private:
		Mix_Chunk* m_pMixChunk{};
		std::string m_FileName{};
		int m_DesiredChannel{-1};
		int m_CurrentChannel{-1};
	};
}