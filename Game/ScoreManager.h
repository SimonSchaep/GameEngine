#pragma once
#include "BaseComponent.h"
#include "Observer.h"
#include "EventTypes.h"
#include "ObservingPointer.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

class SpecialPickupLogic;
class GameManager;
class EnemyLogic;

class ScoreManager : public engine::BaseComponent
	, public engine::Observer<EventType>
	, public engine::Observer<engine::GameObject*, EventType>
	, public engine::Observer<EventType, EnemyLogic*>
	, public engine::Observer<EventType, SpecialPickupLogic*>	
{
public:
	ScoreManager(engine::GameObject* pGameObject, const std::string& highscoresFilename);
	virtual ~ScoreManager();

	virtual void Initialize() override;

	virtual void Notify(EventType type)override;
	virtual void Notify(engine::GameObject*, EventType type)override;
	virtual void Notify(EventType type, EnemyLogic* pEnemyLogic)override;
	virtual void Notify(EventType type, SpecialPickupLogic* pickupLogic)override;

	std::vector<std::pair<int, std::string>> GetHighscores();
	void SaveHighScore(const std::string& name);

	int GetScore()const { return m_Score; }

private:
	void AddScore(int amount, const glm::vec2& position);

	int m_Score{};

	int m_LivesAdded{};

	engine::ObservingPointer<GameManager> m_GameManager{};

	std::string m_HighscoresFileName{};
	std::vector<std::pair<int, std::string>> m_Highscores{};
};