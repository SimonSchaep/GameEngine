#pragma once
#include "Logger.h"

namespace engine
{
	class NullLogger final : public Logger
	{
	public:
		NullLogger() = default;

	private:
		virtual void Log(const std::string&, LogType = LogType::message)override {};
		virtual void LogLine(const std::string&, LogType = LogType::message)override {};
	};

}