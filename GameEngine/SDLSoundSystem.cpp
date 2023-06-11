#include "SDLSoundSystem.h"
#include "SDLSoundClip.h"
#include "Logger.h"
#include "ServiceLocator.h"
#include <SDL_mixer.h>
#include <assert.h>

class engine::SDLSoundSystem::SDLSoundSystemImpl
{
	struct SoundToProcess
	{
		SDLSoundClip* pSoundClip{};
		SoundAction soundAction{};
		int volume{};
	};
public:
	SDLSoundSystemImpl()
		:m_SoundClips{}
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, m_ChannelCount, 2048) < 0)
		{
			std::string msg = "Error opening sdl audio: " + std::string(Mix_GetError());
			ServiceLocator::GetLogger().LogLine(msg, LogType::error);
			return;
		}
		Mix_AllocateChannels(m_ChannelCount);

		m_SoundThread = std::jthread(&SDLSoundSystemImpl::ProcessSounds, this);
	}
	~SDLSoundSystemImpl()
	{
		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_EndQueue = true;
		m_DoesQueueNeedProcessing.notify_one();
		Mix_CloseAudio();
	}

	int AddClip(const std::string& fileName)
	{
		m_SoundClips.emplace_back(std::make_unique<SDLSoundClip>(fileName));
		
		return int(m_SoundClips.size() - 1);
	}

	void Play(int clipId)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.emplace_back(SoundToProcess{ m_SoundClips[clipId].get(), SoundAction::play });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void Stop(int clipId)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.emplace_back(SoundToProcess{ m_SoundClips[clipId].get(), SoundAction::stop });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void Pause(int clipId)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.emplace_back(SoundToProcess{ m_SoundClips[clipId].get(), SoundAction::pause });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void Resume(int clipId)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.emplace_back(SoundToProcess{ m_SoundClips[clipId].get(), SoundAction::resume });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void SetVolume(int clipId, int volume)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.emplace_back(SoundToProcess{ m_SoundClips[clipId].get(), SoundAction::setvolume, volume });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void Mute()
	{
		SetGlobalVolume(0);
	}

	void UnMute()
	{
		SetGlobalVolume(m_MasterVolume);
	}

	void SetGlobalVolume(int volume)
	{
		if (volume != 0) //make sure we don't overwrite mastervolume when muting, since than we wouldn't be able to unmute
		{
			m_MasterVolume = volume;
		}

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.emplace_back(SoundToProcess{ nullptr, SoundAction::setglobalvolume, volume });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void StopAll()
	{
		if (m_SoundClips.size() == 0)return;
		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.emplace_back(SoundToProcess{ nullptr, SoundAction::stopall});
		m_DoesQueueNeedProcessing.notify_one();
	}


private:
	void ProcessSounds()
	{
		while (!m_EndQueue)
		{
			std::unique_lock<std::mutex> lock(m_SoundsToProcessMutex); //unique because it needs to be used with a condition variable
			m_DoesQueueNeedProcessing.wait(lock, [this] { return !m_SoundsToProcess.empty() || m_EndQueue; });

			if (m_EndQueue)
			{
				break;
			}

			auto pSoundClip = m_SoundsToProcess[0].pSoundClip;
			auto soundAction = m_SoundsToProcess[0].soundAction;
			int volume = m_SoundsToProcess[0].volume;

			m_SoundsToProcess.erase(m_SoundsToProcess.begin());

			lock.unlock();

			switch (soundAction)
			{
			case SoundAction::play:
				if (!pSoundClip->IsLoaded())
				{
					pSoundClip->Load();
				}
				pSoundClip->Play();
				break;
			case SoundAction::stop:
				pSoundClip->Stop();
				break;
			case SoundAction::pause:
				pSoundClip->Pause();
				break;
			case SoundAction::resume:
				pSoundClip->Resume();
				break;
			case SoundAction::setvolume:
				pSoundClip->SetVolume(volume);
				break;
			case SoundAction::setglobalvolume:
				Mix_MasterVolume(volume);
				break;
			case SoundAction::stopall:
				Mix_HaltChannel(-1);
				break;
			}
		}
	}

	std::vector<std::unique_ptr<SDLSoundClip>> m_SoundClips;

	std::vector<SoundToProcess> m_SoundsToProcess{};

	int m_ChannelCount{ 8 };

	std::jthread m_SoundThread{};
	std::condition_variable m_DoesQueueNeedProcessing{};
	std::mutex m_SoundsToProcessMutex{};

	bool m_EndQueue{};

	int m_MasterVolume{};
};

engine::SDLSoundSystem::SDLSoundSystem()
{
	m_Impl = std::make_unique<SDLSoundSystemImpl>();
}

engine::SDLSoundSystem::~SDLSoundSystem() = default;

int engine::SDLSoundSystem::AddClip(const std::string& fileName)
{
	return m_Impl->AddClip(fileName);
}

void engine::SDLSoundSystem::Play(int clipId)
{
	m_Impl->Play(clipId);
}

void engine::SDLSoundSystem::Stop(int clipId)
{
	m_Impl->Stop(clipId);
}

void engine::SDLSoundSystem::Pause(int clipId)
{
	m_Impl->Pause(clipId);
}

void engine::SDLSoundSystem::Resume(int clipId)
{
	m_Impl->Resume(clipId);
}

void engine::SDLSoundSystem::SetVolume(int clipId, int volume)
{
	m_Impl->SetVolume(clipId, volume);
}

void engine::SDLSoundSystem::Mute()
{
	m_Impl->Mute();
}

void engine::SDLSoundSystem::UnMute()
{
	m_Impl->UnMute();
}

void engine::SDLSoundSystem::SetGlobalVolume(int volume)
{
	m_Impl->SetGlobalVolume(volume);
}

void engine::SDLSoundSystem::StopAll()
{
	m_Impl->StopAll();
}
