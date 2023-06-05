#pragma once

namespace engine
{

	class BaseComponent;

	class BaseCommand
	{
	public:
		BaseCommand() = default;
		virtual ~BaseCommand() = default;
		virtual bool Execute() = 0;
	};

}