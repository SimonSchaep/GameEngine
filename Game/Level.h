#pragma once
#include "BaseComponent.h"
#include "glm/glm.hpp"
#include <string>
#include <vector>

//Forward Declarations
namespace engine
{
	class Scene;
	class GameObject;
	class BoxCollider;
	class TextureRenderComponent;
}

namespace levelParser
{
	struct LevelElement;
	enum class ELevelElement;
}

//todo: should somehow be able to set cell size, and level pos

class Level : public engine::BaseComponent
{
public:
	Level(engine::GameObject* pGameObject, const std::string& levelFileName);
	virtual ~Level() = default;

	virtual void Initialize() override;
	virtual void Update() override {};

	bool IsNavigableByPlayer(int row, int col)const;
	bool IsNavigableByEnemy(int row, int col)const;

	bool IsInCenterOfElementX(const glm::vec2& pos, int margin)const;
	bool IsInCenterOfElementY(const glm::vec2& pos, int margin)const;
	void SnapToCenterX(glm::vec2& pos)const;
	void SnapToCenterY(glm::vec2& pos)const;
	void GetRowColOfPos(const glm::vec2& pos, int& row, int& col)const;
	glm::vec2 GetCenterOfCell(int row, int col)const;

	int GetLevelWidth()const { return m_LevelWidth; }
	int GetLevelHeight()const { return m_LevelHeight; }
	float GetCellWidth()const { return m_GridElementWidth; }
	float GetCellHeight()const { return m_GridElementHeight; }
	const glm::vec2& GetLevelPosition()const { return m_LevelStartPos; }

private:
	void BuildLevel();
	int GetLevelElementsIndex(int row, int col);

	void CreateDarkPlatform(engine::TextureRenderComponent* pRenderComponent, engine::GameObject* pLevelElementGameObject, bool hasLadder);
	void SpawnPlate(glm::vec2 pos, const std::vector<levelParser::LevelElement>& levelElements, int row, int col);
	void SpawnFood(glm::vec2 pos, const std::vector<levelParser::LevelElement>& levelElements, int row, int col, levelParser::ELevelElement eLevelElement, const std::string& name);
	void CreateFoodElement(const glm::vec2 pos, const std::string& textureFileName, engine::GameObject* pParent, engine::BoxCollider* pParentCollider);

	void GenerateNavigableAreas(const std::vector<levelParser::LevelElement>& levelElements);
	std::vector<bool> m_PlayerNavigableArea{};
	std::vector<bool> m_EnemyNavigableArea{};

	int m_LevelWidth{};
	int m_LevelHeight{};

	float m_GridElementWidth{ 32 };
	float m_GridElementHeight{ 24 };
	glm::vec2 m_LevelStartPos{ 80,80 };

	std::string m_LevelFileName{};
};

