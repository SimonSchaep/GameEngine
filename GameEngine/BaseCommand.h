#pragma once

class BaseComponent;

class BaseCommand
{
public:
	BaseCommand() = default;
	virtual ~BaseCommand() = default;
	virtual void Execute() = 0;
};