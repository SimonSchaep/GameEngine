#pragma once

class BaseComponent;

class BaseAxisCommand
{
public:
	BaseAxisCommand() = default;
	virtual ~BaseAxisCommand() = default;
	virtual void Execute(float axisValue) = 0;
};

