#pragma once

namespace engine
{

	class BaseComponent;

	//some commands needs to somehow get the axis value, only way that I see that it's possible, is by passing it in the execute
	class BaseAxisCommand
	{
	public:
		BaseAxisCommand() = default;
		virtual ~BaseAxisCommand() = default;
		virtual bool Execute(float axisValue) = 0;
	};

}