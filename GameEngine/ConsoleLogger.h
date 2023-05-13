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

	private:
		std::string m_MessagePrefix{ "" };
		std::string m_DebugPrefix{ "DEBUG: " };
		std::string m_WarningPrefix{ "WARNING: " };
		std::string m_ErrorPrefix{ "ERROR: " };

		//messageColor
		//debugColor
		//warningColor
		//errorColor
	};

}