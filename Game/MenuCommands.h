#pragma once
#include "BaseCommand.h"
#include "GameState.h"
#include "StartMenuState.h"
#include "GamePlayingState.h"
#include "GamePausedState.h"
#include "LeaderboardState.h"
#include "ObservingPointer.h"
#include "GameManager.h"

class StartGameCommand final : public engine::BaseCommand
{
public:
	StartGameCommand(StartMenuState* pStartMenuState, GameMode gameMode)
		: m_pStartMenuState{ pStartMenuState }
		, m_GameMode{ gameMode }
	{
	}

	virtual ~StartGameCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pStartMenuState)return false;
		m_pStartMenuState->StartGame(m_GameMode);
		return true;
	}

private:
	engine::ObservingPointer<StartMenuState> m_pStartMenuState;
	GameMode m_GameMode;
};


class PauseGameCommand final : public engine::BaseCommand
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


class ResumeGameCommand final : public engine::BaseCommand
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


class PauseToMenuCommand final : public engine::BaseCommand
{
public:
	PauseToMenuCommand(GamePausedState* pGamePausedState)
		: m_pGamePausedState{ pGamePausedState }
	{
	}

	virtual ~PauseToMenuCommand() = default;

	virtual bool Execute() override
	{
		if (!m_pGamePausedState)return false;
		m_pGamePausedState->BackToMenu();
		return true;
	}

private:
	engine::ObservingPointer<GamePausedState> m_pGamePausedState;
};


class LeaderboardToMenuCommand final : public engine::BaseCommand
{
public:
	LeaderboardToMenuCommand(LeaderboardState* pLeaderboardState)
		: m_LeaderboardState{ pLeaderboardState }
	{
	}

	virtual ~LeaderboardToMenuCommand() = default;

	virtual bool Execute() override
	{
		if (!m_LeaderboardState)return false;
		m_LeaderboardState->BackToMenu();
		return true;
	}

private:
	engine::ObservingPointer<LeaderboardState> m_LeaderboardState;
};


class EndGameCommand final : public engine::BaseCommand
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


class NextLevelCommand final : public engine::BaseCommand
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