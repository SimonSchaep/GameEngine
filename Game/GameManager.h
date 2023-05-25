#pragma once
#include <BaseComponent.h>
#include <vector>

class GameState;
class StartMenuState;

class GameManager final : public engine::BaseComponent
{
public:
	GameManager(engine::GameObject* pGameObject);

	virtual void Initialize() override;
	virtual void Update() override;

private:
	std::unique_ptr<StartMenuState> m_StartMenuState{};

	GameState* m_pActiveGameState{};
};

