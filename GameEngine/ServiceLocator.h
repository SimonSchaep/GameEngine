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

		template<typename T, typename ...Args>
		static T* RegisterLogger(Args... args);

		template<typename T, typename ...Args>
		static T* RegisterSoundSystem(Args... args);

	private:
		static std::unique_ptr<Logger> m_Logger;
		static std::unique_ptr<SoundSystem> m_SoundSystem;
	};

	template<typename T, typename ...Args>
	static T* ServiceLocator::RegisterLogger(Args... args)
	{
		auto logger = std::make_unique<T>(args...);
		T* pReturnValue = logger.get();
		m_Logger = std::move(logger);
		return pReturnValue;
	}

	template<typename T, typename ...Args>
	static T* ServiceLocator::RegisterSoundSystem(Args... args)
	{
		auto soundSystem = std::make_unique<T>(args...);
		T* pReturnValue = soundSystem.get();
		m_SoundSystem = std::move(soundSystem);
		return pReturnValue;
	}
}