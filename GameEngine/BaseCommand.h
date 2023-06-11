#pragma once

namespace engine
{
	class BaseCommand
	{
	public:
		BaseCommand() = default;
		virtual ~BaseCommand() = default;
		virtual bool Execute() = 0;
	};

}