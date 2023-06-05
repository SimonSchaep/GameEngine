#pragma once
#include "Logger.h"

//todo: add color 
//colorizing text: https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

namespace engine
{
	class ConsoleLogger final : public Logger
	{
	public:
		ConsoleLogger() = default;

		virtual void Log(const std::string& message, LogType logType = LogType::message)override;
		virtual void LogLine(const std::string& message, LogType logType = LogType::message)override;

		void SetEnableTimestamps(bool timestampsEnabled) { m_TimeStampsEnabled = timestampsEnabled; }

	private:
		std::string GetTimeStamp();

		bool m_TimeStampsEnabled{};

		std::string m_MessagePrefix{ "MESSAGE: " };
		std::string m_DebugPrefix{ "DEBUG: " };
		std::string m_WarningPrefix{ "WARNING: " };
		std::string m_ErrorPrefix{ "ERROR: " };

		unsigned short m_DefaultColor{ 7 };

		unsigned short m_MessageColor{ 7 };
		unsigned short m_DebugColor{ 7 };
		unsigned short m_WarningColor{ 6 };
		unsigned short m_ErrorColor{ 4 };
	};

}