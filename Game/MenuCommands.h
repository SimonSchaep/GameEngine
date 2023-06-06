#pragma once
#include "BaseCommand.h"
#include "GameState.h"
#include "ObservingPointer.h"

class StartGameCommand : public engine::BaseCommand
{
public:
	StartGameCommand(StartMenuState* pStartMenuState)
		: m_pStartMenuState{ pStartMenuState }
	{
	}

	virtual ~StartGameCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pStartMenuState)return false;
		m_pStartMenuState->StartGame();
		return true;
	}

private:
	engine::ObservingPointer<StartMenuState> m_pStartMenuState;
};


class PauseGameCommand : public engine::BaseCommand
{
public:
	PauseGameCommand(GamePlayingState* pGamePlayingState)
		: m_pGamePlayingState{ pGamePlayingState }
	{
	}

	virtual ~PauseGameCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pGamePlayingState)return false;
		m_pGamePlayingState->PauseGame();
		return true;
	}

private:
	engine::ObservingPointer<GamePlayingState> m_pGamePlayingState;
};


class ResumeGameCommand : public engine::BaseCommand
{
public:
	ResumeGameCommand(GamePausedState* pGamePausedState)
		: m_pGamePausedState{ pGamePausedState }
	{
	}

	virtual ~ResumeGameCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pGamePausedState)return false;
		m_pGamePausedState->ResumeGame();
		return true;
	}

private:
	engine::ObservingPointer<GamePausedState> m_pGamePausedState;
};


class EndGameCommand : public engine::BaseCommand
{
public:
	EndGameCommand(GamePlayingState* pGamePlayingState)
		: m_pGamePlayingState{ pGamePlayingState }
	{
	}

	virtual ~EndGameCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pGamePlayingState)return false;
		m_pGamePlayingState->EndGame();
		return true;
	}

private:
	engine::ObservingPointer<GamePlayingState> m_pGamePlayingState;
};


class NextLevelCommand : public engine::BaseCommand
{
public:
	NextLevelCommand(GameManager* pGameManager)
		: m_pGameManager{ pGameManager }
	{
	}

	virtual ~NextLevelCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pGameManager)return false;
		m_pGameManager->StartNextLevel();
		return true;
	}

private:
	engine::ObservingPointer<GameManager> m_pGameManager;
};