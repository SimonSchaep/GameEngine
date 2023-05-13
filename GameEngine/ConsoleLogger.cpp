#include "ConsoleLogger.h"
#include <iostream>

void engine::ConsoleLogger::Log(const std::string& message, LogType logType)
{
#ifndef _DEBUG
	if (logType == LogType::debug)return;
#endif // _DEBUG

	switch (logType)
	{
	case engine::LogType::message:
		std::cout << m_MessagePrefix << message;
		break;
	case engine::LogType::debug:
		std::cout << m_DebugPrefix << message;
		break;
	case engine::LogType::warning:
		std::cout << m_WarningPrefix << message;
		break;
	case engine::LogType::error:
		std::cout << m_ErrorPrefix << message;
		break;
	}
}

void engine::ConsoleLogger::LogLine(const std::string& message, LogType logType)
{
#ifndef _DEBUG
	if (logType == LogType::debug)return;
#endif // _DEBUG

	Log(message, logType);
	std::cout << '\n';
}
