#include "ScoreManager.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GamePlayingState.h"
#include "SpecialPickupLogic.h"
#include "Scene.h"
#include "FoodParent.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "EnemyLogic.h"
#include "TextRenderComponent.h"
#include "LifeTimer.h"
#include "AutoMoveComponent.h"
#include "LayersEnum.h"
#include <filesystem>
#include <fstream>

using namespace engine;

ScoreManager::ScoreManager(engine::GameObject* pGameObject, const std::string& highscoresFilename)
	:BaseComponent(pGameObject)
	, m_HighscoresFileName{highscoresFilename}
{
}

ScoreManager::~ScoreManager() = default;

void ScoreManager::Initialize()
{
	m_GameManager = GetGameObject()->GetComponent<GameManager>();
	m_GameManager->GetOnSpawnPickup()->AddObserver(this);
	m_GameManager->GetOnStartNextLevel()->AddObserver(this);
}

void ScoreManager::Reset()
{
	m_LivesAdded = 0;
	m_Score = 0;
}

void ScoreManager::Notify(EventType type)
{
	if (type == EventType::startNextLevel)
	{
		//sub to food fall events
		auto foodObjects = GetScene()->FindAllGameObjectsWithTag("foodparent");
		for (auto pObject : foodObjects)
		{
			auto pFood = pObject->GetComponent<FoodParent>();
			assert(pFood != nullptr); //if this fails, some object has tag "foodparent" but no foodparent component
			pFood->GetFallEvent()->AddObserver(this);
		}
		//sub to enemy die events
		auto enemyObjects = GetScene()->FindAllGameObjectsWithTag("Enemy");
		for (auto pObject : enemyObjects)
		{
			auto pEnemy = pObject->GetComponent<EnemyLogic>();
			assert(pEnemy != nullptr); //if this fails, some object has tag "enemy" but no enemy logic component
			pEnemy->GetOnDeath()->AddObserver(this);
		}
	}
}

void ScoreManager::Notify(engine::GameObject* pObject, EventType type)
{
	if (type == EventType::foodStopFall)
	{
		auto pFood = pObject->GetComponent<FoodParent>();
		int enemyCount = pFood->GetEnemiesOnTopCount();
		if (enemyCount == 0)
		{
			AddScore(50, pObject->GetTransform()->GetWorldPosition());
		}
		else if(enemyCount == 1)
		{
			AddScore(500, pObject->GetTransform()->GetWorldPosition());
		}
		else
		{
			ServiceLocator::GetLogger().LogLine("enemy count: " + std::to_string(enemyCount));
			AddScore(int(pow(2, enemyCount-2)*1000), pObject->GetTransform()->GetWorldPosition());
		}
	}
}

void ScoreManager::Notify(EventType type, EnemyLogic* pEnemyLogic)
{
	if (type == EventType::enemyDied)
	{
		auto& name = pEnemyLogic->GetGameObject()->GetName();
		if (name == "Hotdog")
		{
			AddScore(100, pEnemyLogic->GetTransform()->GetWorldPosition());
		}
		else if (name == "Pickle")
		{
			AddScore(200, pEnemyLogic->GetTransform()->GetWorldPosition());
		}
		else if (name == "Egg")
		{
			AddScore(300, pEnemyLogic->GetTransform()->GetWorldPosition());
		}
	}
}

void ScoreManager::Notify(EventType type, SpecialPickupLogic* pickupLogic)
{
	if (type == EventType::spawnedPickup)
	{
		pickupLogic->GetOnPickup()->AddObserver(this);
	}
	else if (type == EventType::iceCreamPickedUp)
	{
		AddScore(500, pickupLogic->GetTransform()->GetWorldPosition());
	}
	else if (type == EventType::coffeePickedUp)
	{
		AddScore(1000, pickupLogic->GetTransform()->GetWorldPosition());
	}
	else if (type == EventType::friesPickedUp)
	{
		AddScore(1500, pickupLogic->GetTransform()->GetWorldPosition());
	}
}

std::vector<std::pair<int, std::string>> ScoreManager::GetHighscores()
{
	if (m_Highscores.size() == 0 && std::filesystem::exists(m_HighscoresFileName)) //not loaded yet
	{
		std::ifstream ifStream{m_HighscoresFileName};

		std::string line{};

		while (std::getline(ifStream, line))
		{
			std::stringstream sstream{ line };
			std::string name{};
			std::string scoreString{};
			std::getline(sstream, name, ':');
			std::getline(sstream, scoreString);

			if (scoreString == "") //could be empty on the last line
			{
				continue;
			}

			int score = std::stoi(scoreString);
			m_Highscores.emplace_back(std::pair<int, std::string>{ score, name });
		}
	}

	return m_Highscores;
}

void ScoreManager::SaveHighScore(const std::string& name)
{
	if (m_Score == 0)return; //no point saving empty score

	GetHighscores(); //make sure highscores are loaded so we don't overwrite

	m_Highscores.emplace_back(std::pair<int, std::string>{ m_Score, name });
	m_Score = 0;

	std::sort(m_Highscores.begin(), m_Highscores.end(), [](const std::pair<int, std::string>& pair1, const std::pair<int, std::string>& pair2)
		{
			return pair1.first > pair2.first;
		});

	std::filesystem::path path{ m_HighscoresFileName }; //make sure we only have the path
	if (!std::filesystem::exists(path.parent_path()))
	{
		std::filesystem::create_directory(path.parent_path());
	}
	std::ofstream ofStream{m_HighscoresFileName};

	for (auto& pair : m_Highscores)
	{
		ofStream << pair.second << ':' << pair.first << '\n';
	}
}


void ScoreManager::AddScore(int amount, const glm::vec2& position)
{
	glm::vec2 offset{10,30};

	m_Score += amount;
	if (m_Score >= 20000 + (20000 * m_LivesAdded))
	{
		m_GameManager->GetGamePlayingState()->AddLife();
		m_LivesAdded++;
	}

	auto pScoreObject = GetScene()->CreateAndAddGameObject("ScoreText");
	pScoreObject->GetTransform()->SetWorldPosition(position + offset);
	pScoreObject->CreateAndAddComponent<LifeTimer>(2.f);
	pScoreObject->CreateAndAddComponent<AutoMoveComponent>(10.f, glm::vec2{0,1});
	auto pText = pScoreObject->CreateAndAddComponent<TextRenderComponent>();
	pText->SetLayer(Layer::hud);
	pText->SetFont("super-burger-time.ttf", 15);
	pText->SetText(std::to_string(amount));

	ServiceLocator::GetLogger().LogLine("");
	ServiceLocator::GetLogger().LogLine("Added: " + std::to_string(amount));
	ServiceLocator::GetLogger().LogLine("Score: " + std::to_string(m_Score));
	ServiceLocator::GetLogger().LogLine("");
}