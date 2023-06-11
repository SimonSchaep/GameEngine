#pragma once
#include "UIRenderComponent.h"
#include "Event.h"
#include "EventTypes.h"
#include <string>
#include <memory>

class NameInputBox : public engine::UIRenderComponent
{
public:
	NameInputBox(engine::GameObject* pGameObject);
	virtual ~NameInputBox() = default;

	virtual void RenderUI() override;

	virtual void Update()override;

	engine::Event<EventType, const std::string&>* GetOnNameEntered()const { return m_OnNameEntered.get(); }

private:
	std::string m_EnteredName{};

	std::unique_ptr<engine::Event<EventType, const std::string&>> m_OnNameEntered{};

	bool m_ButtonPressed{};
};

