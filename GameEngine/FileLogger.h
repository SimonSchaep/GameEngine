#pragma once
#include "Logger.h"
#include <fstream>

//todo: add timestamps

namespace engine
{
	class FileLogger : public Logger
	{
	public:
		FileLogger();

		template<typename T>
		void SetActualLogger();
		void SetFilePath(const std::string& filePath);

		virtual void Log(const std::string& message, LogType logType = LogType::message)override;
		virtual void LogLine(const std::string& message, LogType logType = LogType::message)override;

	private:
		void LogText(const std::string& message, LogType logType);

		std::unique_ptr<Logger> m_ActualLogger{};

		std::string m_FilePath{"Logs/log.txt"};

		std::ofstream m_OfStream{};

		std::string m_MessagePrefix{ "" };
		std::string m_DebugPrefix{ "DEBUG: " };
		std::string m_WarningPrefix{ "WARNING: " };
		std::string m_ErrorPrefix{ "ERROR: " };
	};

	template<typename T>
	void FileLogger::SetActualLogger()
	{
		m_ActualLogger = std::make_unique<T>();
	}
}