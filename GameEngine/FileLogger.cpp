#include "FileLogger.h"
#include "NullLogger.h"
#include <filesystem>

engine::FileLogger::FileLogger()
{
	m_ActualLogger = std::make_unique<NullLogger>();
	SetFilePath(m_FilePath);
}

void engine::FileLogger::SetFilePath(const std::string& filePath)
{	
	m_FilePath = filePath;
	
	std::filesystem::path path{ m_FilePath }; //make sure we only have the path
	if (!std::filesystem::exists(path.parent_path()))
	{
		std::filesystem::create_directory(path.parent_path());
	}
	m_OfStream.open(m_FilePath, std::ios_base::app);
}

void engine::FileLogger::Log(const std::string& message, LogType logType)
{
	m_ActualLogger->Log(message, logType);

#ifndef _DEBUG
	if (logType == LogType::debug)return;
#endif // _DEBUG

	LogText(message, logType);
}

void engine::FileLogger::LogLine(const std::string& message, LogType logType)
{
	m_ActualLogger->LogLine(message, logType);

#ifndef _DEBUG
	if (logType == LogType::debug)return;
#endif // _DEBUG

	LogText(message, logType);

	m_OfStream << '\n';
}

void engine::FileLogger::LogText(const std::string& message, LogType logType)
{
	switch (logType)
	{
	case engine::LogType::message:
		m_OfStream << m_MessagePrefix << message;
		break;
	case engine::LogType::debug:
		m_OfStream << m_DebugPrefix << message;
		break;
	case engine::LogType::warning:
		m_OfStream << m_WarningPrefix << message;
		break;
	case engine::LogType::error:
		m_OfStream << m_ErrorPrefix << message;
		break;
	}
}
