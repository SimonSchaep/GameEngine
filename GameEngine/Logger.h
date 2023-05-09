#pragma once
#include <string>

//todo: finish this
//colorizing text: https://stackoverflow.com/questions/4053837/colorizing-text-in-the-console-with-c

namespace engine
{
	enum class LogType
	{
		message,
		debug,
		warning,
		error,
	};

	class Logger
	{
	public:

		void SetIsFileLoggingEnabled(bool isEnabled) { m_IsFileLoggingEnabled = isEnabled; }
		void Log(const std::string& message, LogType logType = LogType::message);

	private:
		std::string m_LogFileLocation{};
		bool m_IsFileLoggingEnabled{};

		//messageColor
		//debugColor
		//warningColor
		//errorColor

	};

}


