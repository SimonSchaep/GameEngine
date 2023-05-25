#pragma once
#include "BaseCommand.h"
#include "GameState.h"

class StartGameCommand : public engine::BaseCommand
{
public:
	StartGameCommand(StartMenuState* pStartMenuState)
		: m_pStartMenuState{ pStartMenuState }
	{
	}

	virtual ~StartGameCommand() = default;

	virtual void Execute() override
	{
		m_pStartMenuState->StartGame();
	}

private:
	StartMenuState* m_pStartMenuState;
};


class PauseGameCommand : public engine::BaseCommand
{
public:
	PauseGameCommand(GamePlayingState* pGamePlayingState)
		: m_pGamePlayingState{ pGamePlayingState }
	{
	}

	virtual ~PauseGameCommand() = default;

	virtual void Execute() override
	{
		m_pGamePlayingState->PauseGame();
	}

private:
	GamePlayingState* m_pGamePlayingState;
};


class ResumeGameCommand : public engine::BaseCommand
{
public:
	ResumeGameCommand(GamePausedState* pGamePausedState)
		: m_pGamePausedState{ pGamePausedState }
	{
	}

	virtual ~ResumeGameCommand() = default;

	virtual void Execute() override
	{
		m_pGamePausedState->ResumeGame();
	}

private:
	GamePausedState* m_pGamePausedState;
};


class EndGameCommand : public engine::BaseCommand
{
public:
	EndGameCommand(GamePlayingState* pGamePlayingState)
		: m_pGamePlayingState{ pGamePlayingState }
	{
	}

	virtual ~EndGameCommand() = default;

	virtual void Execute() override
	{
		m_pGamePlayingState->EndGame();
	}

private:
	GamePlayingState* m_pGamePlayingState;
};