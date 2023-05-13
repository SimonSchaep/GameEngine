#include "SDLSoundClip.h"
#include "Logger.h"
#include "ServiceLocator.h"
#include <SDL_mixer.h>

engine::SDLSoundClip::SDLSoundClip(const std::string& fileName)
	:m_FileName{fileName}
{}

engine::SDLSoundClip::~SDLSoundClip()
{
	Mix_FreeChunk(m_pMixChunk);
	m_pMixChunk = nullptr;
}

bool engine::SDLSoundClip::IsLoaded() const
{
	return m_pMixChunk;
}

void engine::SDLSoundClip::Load()
{
	m_pMixChunk = Mix_LoadWAV(m_FileName.c_str());
	if (!m_pMixChunk)
	{
		std::string msg = "sound file " + m_FileName + " not found, SDL_mixer Error: " + Mix_GetError();
		ServiceLocator::GetLogger().LogLine(msg, LogType::error);
	}
}

void engine::SDLSoundClip::Play()
{
	if (IsLoaded())
	{
		m_CurrentChannel = Mix_PlayChannel(m_DesiredChannel, m_pMixChunk, 0); //set active channel to channel it was played on
	}	
}

void engine::SDLSoundClip::Stop()
{
	if (IsPlaying())
	{
		Mix_HaltChannel(m_CurrentChannel);
	}	
}

bool engine::SDLSoundClip::IsPlaying() const
{
	if (IsLoaded())
	{
		return Mix_Playing(m_CurrentChannel);
	}
	return false;
}

void engine::SDLSoundClip::Pause()
{
	if (IsLoaded())
	{
		Mix_Pause(m_CurrentChannel);
	}
}

void engine::SDLSoundClip::Resume()
{
	if (IsLoaded())
	{
		Mix_Resume(m_CurrentChannel);
	}
}

void engine::SDLSoundClip::SetVolume(int volume)
{
	if (IsLoaded())
	{		
		Mix_VolumeChunk(m_pMixChunk, volume);
	}	
}

int engine::SDLSoundClip::GetVolume() const
{
	if (IsLoaded())
	{
		return Mix_VolumeChunk(m_pMixChunk, -1);
	}
	return -1;
}
