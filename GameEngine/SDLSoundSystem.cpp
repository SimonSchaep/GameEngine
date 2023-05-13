#include "SDLSoundSystem.h"
#include "SDLSoundClip.h"
#include "Logger.h"
#include "ServiceLocator.h"
#include <SDL_mixer.h>
#include <assert.h>

class engine::SDLSoundSystem::SDLSoundSystemImpl
{
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
		m_SoundClips.push_back(std::make_unique<SDLSoundClip>(fileName));
		return int(m_SoundClips.size() - 1);
	}

	void Play(int clipId)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.push_back(std::pair<SDLSoundClip*, SoundAction>{ m_SoundClips[clipId].get(), SoundAction::play });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void Stop(int clipId)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.push_back(std::pair<SDLSoundClip*, SoundAction>{ m_SoundClips[clipId].get(), SoundAction::stop });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void Pause(int clipId)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.push_back(std::pair<SDLSoundClip*, SoundAction>{ m_SoundClips[clipId].get(), SoundAction::pause });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void Resume(int clipId)
	{
		assert(clipId < int(m_SoundClips.size()));

		std::lock_guard<std::mutex> lock(m_SoundsToProcessMutex);
		m_SoundsToProcess.push_back(std::pair<SDLSoundClip*, SoundAction>{ m_SoundClips[clipId].get(), SoundAction::resume });
		m_DoesQueueNeedProcessing.notify_one();
	}

	void StopAll()
	{
		Mix_HaltChannel(-1);
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

			auto soundClip = m_SoundsToProcess[0].first;
			auto soundAction = m_SoundsToProcess[0].second;

			m_SoundsToProcess.erase(m_SoundsToProcess.begin());

			lock.unlock();

			switch (soundAction)
			{
			case SoundAction::play:
				if (!soundClip->IsLoaded())
				{
					soundClip->Load();
				}
				soundClip->Play();
				break;
			case SoundAction::stop:
				soundClip->Stop();
				break;
			case SoundAction::pause:
				soundClip->Pause();
				break;
			case SoundAction::resume:
				soundClip->Resume();
				break;
			}
		}
	}

	std::vector<std::unique_ptr<SDLSoundClip>> m_SoundClips;

	std::vector<std::pair<SDLSoundClip*, SoundAction>> m_SoundsToProcess{};

	int m_ChannelCount{ 8 };

	std::jthread m_SoundThread{};
	std::condition_variable m_DoesQueueNeedProcessing{};
	std::mutex m_SoundsToProcessMutex{};

	bool m_EndQueue{};
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

void engine::SDLSoundSystem::StopAll()
{
	m_Impl->StopAll();
}
