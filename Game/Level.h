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
	virtual ~Level();

	virtual void Initialize() override;
	virtual void Update() override {};

	bool IsNavigable(int row, int col, bool isEnemy)const;

	int GetIndexFromRowCol(int row, int col)const;
	void GetRowColOfIndex(int index, int& row, int& col)const;

	std::vector<int> GetAdjacentNavigableTiles(int index, bool isEnemy)const;

	bool IsInCenterOfElementX(const glm::vec2& pos, int margin)const;
	bool IsInCenterOfElementY(const glm::vec2& pos, int margin)const;
	void SnapToCenterX(glm::vec2& pos)const;
	void SnapToCenterY(glm::vec2& pos)const;
	void GetRowColOfPos(const glm::vec2& pos, int& row, int& col)const;
	int GetIndexOfPos(const glm::vec2& pos)const;
	glm::vec2 GetCenterOfCell(int row, int col)const;
	glm::vec2 GetCenterOfCell(int index)const;

	int GetLevelWidth()const { return m_LevelWidth; }
	int GetLevelHeight()const { return m_LevelHeight; }
	float GetCellWidth()const { return m_GridElementWidth; }
	float GetCellHeight()const { return m_GridElementHeight; }
	const glm::vec2& GetLevelPosition()const { return m_LevelStartPos; }

private:
	void BuildLevel();

	void CreateDarkPlatform(engine::TextureRenderComponent* pRenderComponent, engine::GameObject* pLevelElementGameObject, bool hasLadder);
	void SpawnPlate(glm::vec2 pos, const std::vector<levelParser::LevelElement>& levelElements, int row, int col);
	void SpawnFood(glm::vec2 pos, const std::vector<levelParser::LevelElement>& levelElements, int row, int col, levelParser::ELevelElement eLevelElement, const std::string& name);
	void CreateFoodElement(const glm::vec2 pos, const std::string& textureFileName, engine::GameObject* pParent, engine::BoxCollider* pParentCollider);

	void GenerateNavigableAreas(const std::vector<levelParser::LevelElement>& levelElements);
	std::vector<bool> m_ChefNavigableArea{};
	std::vector<bool> m_EnemyNavigableArea{};

	int m_LevelWidth{};
	int m_LevelHeight{};

	float m_GridElementWidth{ 32 };
	float m_GridElementHeight{ 24 };
	glm::vec2 m_LevelStartPos{ 80,80 };

	std::string m_LevelFileName{};
};

