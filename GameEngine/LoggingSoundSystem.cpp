#include "LoggingSoundSystem.h"
#include "NullSoundSystem.h"
#include "ServiceLocator.h"
#include "Logger.h"

engine::LoggingSoundSystem::LoggingSoundSystem()
{
	m_ActualSoundSystem = std::make_unique<NullSoundSystem>();
}

int engine::LoggingSoundSystem::AddClip(const std::string& fileName)
{
	int clipId = m_ActualSoundSystem->AddClip(fileName);
	if (clipId == -1)
	{
		ServiceLocator::GetLogger().LogLine("Adding clip " + fileName + " failed", LogType::warning);
	}
	else
	{
		ServiceLocator::GetLogger().LogLine("Adding clip " + fileName + " succeeded. ClipId: " + std::to_string(clipId), LogType::message);
	}
	return clipId;
}

void engine::LoggingSoundSystem::Play(int clipId)
{
	ServiceLocator::GetLogger().LogLine("Playing clip " + std::to_string(clipId), LogType::message);
	m_ActualSoundSystem->Play(clipId);
}

void engine::LoggingSoundSystem::Stop(int clipId)
{
	ServiceLocator::GetLogger().LogLine("Stopping clip " + std::to_string(clipId), LogType::message);
	m_ActualSoundSystem->Stop(clipId);
}

void engine::LoggingSoundSystem::Pause(int clipId)
{
	ServiceLocator::GetLogger().LogLine("Pausing clip " + std::to_string(clipId), LogType::message);
	m_ActualSoundSystem->Pause(clipId);
}

void engine::LoggingSoundSystem::Resume(int clipId)
{
	ServiceLocator::GetLogger().LogLine("Resuming clip " + std::to_string(clipId), LogType::message);
	m_ActualSoundSystem->Resume(clipId);
}

void engine::LoggingSoundSystem::SetVolume(int clipId, int volume)
{
	ServiceLocator::GetLogger().LogLine("Setting volume of clip " + std::to_string(clipId) + " to " + std::to_string(volume), LogType::message);
	m_ActualSoundSystem->SetVolume(clipId, volume);
}

void engine::LoggingSoundSystem::Mute()
{
	ServiceLocator::GetLogger().LogLine("Muting sound system", LogType::message);
	m_ActualSoundSystem->Mute();
}

void engine::LoggingSoundSystem::UnMute()
{
	ServiceLocator::GetLogger().LogLine("Unmuting sound system", LogType::message);
	m_ActualSoundSystem->UnMute();
}

void engine::LoggingSoundSystem::SetGlobalVolume(int volume)
{
	ServiceLocator::GetLogger().LogLine("Setting global volume to " + std::to_string(volume), LogType::message);
	m_ActualSoundSystem->SetGlobalVolume(volume);
}

void engine::LoggingSoundSystem::StopAll()
{
	ServiceLocator::GetLogger().LogLine("Stopping all clips", LogType::message);
	m_ActualSoundSystem->StopAll();
}
