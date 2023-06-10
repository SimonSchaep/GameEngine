#pragma once
#include <string>


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
		virtual ~Logger() = default;

		virtual void Log(const std::string& message, LogType logType = LogType::debug) = 0;
		virtual void LogLine(const std::string& message, LogType logType = LogType::debug) = 0;

	protected:
		Logger() = default;
	};
}