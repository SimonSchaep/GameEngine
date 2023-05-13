#include "ServiceLocator.h"
#include "Logger.h"
#include "NullLogger.h"
#include "SoundSystem.h"
#include "NullSoundSystem.h"

namespace engine
{
	std::unique_ptr<Logger> ServiceLocator::m_Logger = std::make_unique<NullLogger>();
	std::unique_ptr<SoundSystem> ServiceLocator::m_SoundSystem = std::make_unique<NullSoundSystem>();
}