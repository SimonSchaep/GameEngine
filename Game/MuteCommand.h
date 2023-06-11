#pragma once
#include "BaseCommand.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

class MuteCommand final : public engine::BaseCommand
{
public:
	MuteCommand() = default;

	virtual ~MuteCommand() = default;

	virtual bool Execute() override
	{
		m_IsMuted = !m_IsMuted;
		if (m_IsMuted)
		{
			engine::ServiceLocator::GetSoundSystem().Mute();
		}
		else
		{
			engine::ServiceLocator::GetSoundSystem().UnMute();
		}
		return true;
	}

private:
	bool m_IsMuted{};
};