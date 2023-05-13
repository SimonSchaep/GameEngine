#pragma once
#include <memory>

namespace engine
{
	class Logger;
	class SoundSystem;

	class ServiceLocator final
	{
	public:
		static Logger& GetLogger() { return *m_Logger; }
		static SoundSystem& GetSoundSystem() { return *m_SoundSystem; }

		template<typename T>
		static T* RegisterLogger();

		template<typename T>
		static T* RegisterSoundSystem();

	private:
		static std::unique_ptr<Logger> m_Logger;
		static std::unique_ptr<SoundSystem> m_SoundSystem;
	};

	template<typename T>
	static T* ServiceLocator::RegisterLogger()
	{
		auto logger = std::make_unique<T>();
		T* pReturnValue = logger.get();
		m_Logger = std::move(logger);
		return pReturnValue;
	}

	template<typename T>
	static T* ServiceLocator::RegisterSoundSystem()
	{
		auto soundSystem = std::make_unique<T>();
		T* pReturnValue = soundSystem.get();
		m_SoundSystem = std::move(soundSystem);
		return pReturnValue;
	}
}