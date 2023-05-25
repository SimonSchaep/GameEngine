#pragma once
#include <memory>

namespace engine
{
	class GameObject;
}

class GameManager;

class GameState
{
public:
	GameState() = default;
	virtual ~GameState() = default;
	virtual void HandleInput() = 0;
	virtual void Update() = 0;
	virtual void OnEnter() = 0;
	virtual void OnExit() = 0;
private:	
	GameManager* m_pGameManager{};
};

class StartMenuState : public GameState
{
public:
	StartMenuState(engine::GameObject* pMenuGameObject);

	virtual void HandleInput()override {};
	virtual void Update()override;
	virtual void OnEnter()override {};
	virtual void OnExit()override {};
private:

};