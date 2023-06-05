#pragma once
#include "Logger.h"
#include <fstream>

//todo: add timestamps
//todo: use File instead of ostream

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

		void SetEnableTimestamps(bool timestampsEnabled) { m_TimeStampsEnabled = timestampsEnabled; }

	private:
		void LogText(const std::string& message, LogType logType);
		std::string GetTimeStamp();

		bool m_TimeStampsEnabled{ true };

		std::unique_ptr<Logger> m_ActualLogger{};
		std::string m_FilePath{"Logs/log.txt"};
		std::ofstream m_OfStream{};

		std::string m_MessagePrefix{ "MESSAGE: " };
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