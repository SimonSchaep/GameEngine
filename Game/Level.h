#pragma once
#include "Singleton.h"
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

using namespace engine;
using namespace levelParser;

//todo: shouldn't be a singleton
//also should somwhow be able to set cell size, and level pos

class Level final : public engine::Singleton<Level>
{
public:
	void BuildLevel(Scene* pScene, const std::string& fileName);
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
	int GetLevelElementsIndex(int row, int col);

	void CreateDarkPlatform(TextureRenderComponent* pRenderComponent, GameObject* pLevelElementGameObject, bool hasLadder);
	void SpawnPlate(glm::vec2 pos, Scene* pScene, const std::vector<LevelElement>& levelElements, int row, int col);
	void SpawnFood(glm::vec2 pos, Scene* pScene, const std::vector<LevelElement>& levelElements, int row, int col, ELevelElement eLevelElement, const std::string& name);
	void CreateFoodElement(const glm::vec2 pos, Scene* pScene, const std::string& textureFileName, GameObject* pParent, BoxCollider* pParentCollider);

	void GenerateNavigableAreas(const std::vector<LevelElement>& levelElements);
	std::vector<bool> m_PlayerNavigableArea{};
	std::vector<bool> m_EnemyNavigableArea{};

	int m_LevelWidth{};
	int m_LevelHeight{};

	float m_GridElementWidth{ 32 };
	float m_GridElementHeight{ 24 };
	glm::vec2 m_LevelStartPos{ 80,80 };
};

