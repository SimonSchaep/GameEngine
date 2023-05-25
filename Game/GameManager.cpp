#include "GameManager.h"
#include "GameState.h"
#include "GameObject.h"

GameManager::GameManager(engine::GameObject* pGameObject)
	:BaseComponent(pGameObject)
{
	//m_StartMenuState = std::make_unique
}

void GameManager::Initialize()
{

}

void GameManager::Update()
{
	m_pActiveGameState->Update();
}
