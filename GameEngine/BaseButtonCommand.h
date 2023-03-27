#pragma once

class BaseComponent;

class BaseButtonCommand
{
public:
	BaseButtonCommand() = default;
	virtual ~BaseButtonCommand() = default;
	virtual void Execute() = 0;
};