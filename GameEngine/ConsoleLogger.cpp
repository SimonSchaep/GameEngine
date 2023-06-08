#include "ConsoleLogger.h"
#include <windows.h>
#include <iostream>
#include <chrono>

void engine::ConsoleLogger::Log(const std::string& message, LogType logType)
{
#ifndef _DEBUG
	if (logType == LogType::debug)return;
#endif // _DEBUG

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (logType)
	{
	case engine::LogType::message:
		SetConsoleTextAttribute(hConsole, m_MessageColor);
		std::cout << message;
		break;
	case engine::LogType::debug:
		SetConsoleTextAttribute(hConsole, m_DebugColor);
		std::cout << message;
		break;
	case engine::LogType::warning:
		SetConsoleTextAttribute(hConsole, m_WarningColor);
		std::cout << message;
		break;
	case engine::LogType::error:
		SetConsoleTextAttribute(hConsole, m_ErrorColor);
		std::cout << message;
		break;
	}

	SetConsoleTextAttribute(hConsole, m_DefaultColor);
}

void engine::ConsoleLogger::LogLine(const std::string& message, LogType logType)
{
#ifndef _DEBUG
	if (logType == LogType::debug)return;
#endif // _DEBUG

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << GetTimeStamp();
	switch (logType)
	{
	case engine::LogType::message:
		std::cout << m_MessagePrefix;
		break;
	case engine::LogType::debug:
		std::cout << m_DebugPrefix;
		break;
	case engine::LogType::warning:
		std::cout << m_WarningPrefix;
		break;
	case engine::LogType::error:
		std::cout << m_ErrorPrefix;
		break;
	default:
		break;
	}
	Log(message, logType);
	std::cout << '\n';

	SetConsoleTextAttribute(hConsole, m_DefaultColor);
}

std::string engine::ConsoleLogger::GetTimeStamp()
{
	if (!m_TimeStampsEnabled) return "";

	auto now = std::chrono::system_clock::now();
	auto currTime = std::chrono::system_clock::to_time_t(now);
	auto pTm = localtime(&currTime);
	
	return "[" + std::to_string(pTm->tm_hour) + ":" + std::to_string(pTm->tm_min) + ":" + std::to_string(pTm->tm_sec) + "] ";
}
