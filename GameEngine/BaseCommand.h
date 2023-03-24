#pragma once

class BaseComponent;

class BaseCommand
{
protected:
	BaseComponent* GetComponent()const { return m_pComponent; };

public:
	BaseCommand(BaseComponent* pComponent) :m_pComponent{pComponent} {};
	virtual ~BaseCommand() = default;
	virtual void Execute() = 0;
	//example implementation:
	//void MoveRightCommand::Execute()
	//{
	//	  static_cast<MovementComponent*>(GetComponent())->MoveRight();
	//}
	
private:
	BaseComponent* m_pComponent;
};