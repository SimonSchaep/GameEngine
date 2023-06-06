#include "Level.h"
#include "Scene.h"
#include "LevelParser.h"
#include "TextureRenderComponent.h"
#include "BoxCollider.h"
#include "ColliderDebugRenderComponent.h"
#include "FoodParent.h"
#include "ServiceLocator.h"
#include "Logger.h"

using namespace engine;
using namespace levelParser;

Level::Level(engine::GameObject* pGameObject, const std::string& levelFileName)
	:BaseComponent(pGameObject), m_LevelFileName{ levelFileName }
{
}

Level::~Level() = default;

void Level::Initialize()
{
	BuildLevel();
}

bool Level::IsNavigableByPlayer(int row, int col) const
{
	if (col < 0 || col > m_LevelWidth - 1 || row < 0 || row > m_LevelHeight - 1)
	{
		return false;
	}
	return m_PlayerNavigableArea[col + row * m_LevelWidth];
}

bool Level::IsNavigableByEnemy(int row, int col) const
{
	if (col < 0 || col > m_LevelWidth - 1 || row < 0 || row > m_LevelHeight - 1)
	{
		return false;
	}
	return m_EnemyNavigableArea[col + row * m_LevelWidth];
}

bool Level::IsInCenterOfElementX(const glm::vec2& pos, int margin) const
{
	float XPos = pos.x;
	XPos -= m_LevelStartPos.x;
	XPos -= m_GridElementWidth/2;

	return int(XPos) % int(m_GridElementWidth) <= margin || int(XPos) % int(m_GridElementWidth) >= int(m_GridElementWidth) - margin;
}

bool Level::IsInCenterOfElementY(const glm::vec2& pos, int margin) const
{
	float YPos = pos.y;
	YPos -= m_LevelStartPos.y;
	YPos -= m_GridElementHeight / 2;

	return int(YPos) % int(m_GridElementHeight) <= margin || int(YPos) % int(m_GridElementHeight) >= int(m_GridElementHeight) - margin;
}

void Level::SnapToCenterX(glm::vec2& pos) const
{
	pos.x -= m_LevelStartPos.x;
	pos.x -= m_GridElementWidth / 2;	
	int count = int(pos.x) / int(m_GridElementWidth);
	if (int(pos.x) % int(m_GridElementWidth) >= int(m_GridElementWidth) / 2) //closer to next cell center
	{
		++count;
	}
	pos.x = count * m_GridElementWidth;
	pos.x += m_GridElementWidth / 2;
	pos.x += m_LevelStartPos.x;
}

void Level::SnapToCenterY(glm::vec2& pos) const
{
	pos.y -= m_LevelStartPos.y;
	pos.y -= m_GridElementHeight / 2;
	int count = int(pos.y) / int(m_GridElementHeight);
	if (int(pos.y) % int(m_GridElementHeight) >= int(m_GridElementHeight) / 2) //closer to next cell center
	{
		++count;
	}
	pos.y = count * m_GridElementHeight;
	pos.y += m_GridElementHeight / 2;
	pos.y += m_LevelStartPos.y;
}

void Level::GetRowColOfPos(const glm::vec2& pos, int& row, int& col) const
{
	auto gridPos = pos;
	gridPos -= m_LevelStartPos;
	gridPos.x -= m_GridElementWidth / 2;
	gridPos.y -= m_GridElementHeight / 2;
	col = int(gridPos.x) / int(m_GridElementWidth);
	if (int(gridPos.x) % int(m_GridElementWidth) >= int(m_GridElementWidth) / 2) //closer to next cell center
	{
		++col;
	}
	row = int(gridPos.y) / int(m_GridElementHeight);
	if (int(gridPos.y) % int(m_GridElementHeight) >= int(m_GridElementHeight) / 2) //closer to next cell center
	{
		++row;
	}
}

glm::vec2 Level::GetCenterOfCell(int row, int col) const
{
	return m_LevelStartPos + glm::vec2{(col + 0.5f) * m_GridElementWidth, (row + 0.5f) * m_GridElementHeight };
}

void Level::BuildLevel()
{
	auto start = std::chrono::high_resolution_clock::now();

	const glm::vec2 foodOffset{0, -2};

	std::vector<LevelElement> levelElements;

	auto pScene = GetGameObject()->GetScene();

	try
	{
		ParseCsv(m_LevelFileName, levelElements, m_LevelWidth, m_LevelHeight);
	}
	catch (levelParser::FileNameNotFound& except)
	{
		ServiceLocator::GetLogger().LogLine(except.what(), LogType::error);
		return;
	}
	catch (levelParser::InvalidLevelWidth& except)
	{
		ServiceLocator::GetLogger().LogLine(except.what(), LogType::error);
		return;
	}
	catch (levelParser::UnknownLevelElement& except)
	{
		ServiceLocator::GetLogger().LogLine(except.what(), LogType::warning);
	}

	std::vector<bool> hasFoodAbove{ std::vector<bool>(m_LevelWidth, false) }; //to make sure platforms below will be dark blue

	for (int r{}; r < m_LevelHeight; ++r)
	{
		for (int c{}; c < m_LevelWidth; ++c)
		{
			LevelElement levelElement{ levelElements[GetLevelElementsIndex(r, c)] };

			if (levelElement.eLevelElement == ELevelElement::empty && !levelElement.hasLadder) continue;

			glm::vec2 position{ m_LevelStartPos };
			position += glm::vec2{ c* m_GridElementWidth, (m_LevelHeight - 1 - r)* m_GridElementHeight };

			GameObject* pLevelElementGameObject{};
			TextureRenderComponent* pRenderComponent{};
			if (levelElement.eLevelElement != ELevelElement::plate) //plate doesn't have platform behind it
			{
				pLevelElementGameObject = pScene->CreateAndAddGameObject();
				pLevelElementGameObject->GetTransform()->SetLocalPosition(position);
				pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
				pRenderComponent->SetLayer(-1);
				pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
			}

			switch (levelElement.eLevelElement)
			{
			case ELevelElement::empty:
				pRenderComponent->SetTexture("ladder.png");
				break;
			case ELevelElement::platform:
				if (hasFoodAbove[c])
				{
					CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);
				}
				else
				{
					if (levelElement.hasLadder)
					{
						pRenderComponent->SetTexture("platformladder.png");
					}
					else
					{
						pRenderComponent->SetTexture("platform.png");
					}
				}
				break;
			case ELevelElement::topbun:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::topbun) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::topbun, "topbun");
				}
				break;
			case ELevelElement::botbun:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::botbun) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::botbun, "botbun");
				}
				break;
			case ELevelElement::lettuce:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::lettuce) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::lettuce, "lettuce");
				}
				break;
			case ELevelElement::meat:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::meat) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::meat, "meat");
				}
				break;
			case ELevelElement::cheese:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::cheese) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::cheese, "cheese");
				}
				break;
			case ELevelElement::tomato:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::tomato) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::tomato, "tomato");
				}
				break;
			case ELevelElement::plate:
				hasFoodAbove[c] = false;
				if (levelElement.hasLadder)
				{
					ServiceLocator::GetLogger().LogLine("Plate shouldn't have ladder", LogType::warning);
				}
				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::plate) //if this is left most element
				{
					SpawnPlate(position, levelElements, r, c);
				}
				break;
			}
		}
	}

	GenerateNavigableAreas(levelElements);

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
	ServiceLocator::GetLogger().LogLine(("Building level took: " + std::to_string(duration) + "ms"), LogType::message);
}

int Level::GetLevelElementsIndex(int row, int col)
{
	return row * m_LevelWidth + col;
}

void Level::CreateDarkPlatform(TextureRenderComponent* pRenderComponent, GameObject* pLevelElementGameObject, bool hasLadder)
{
	if (hasLadder)
	{
		pRenderComponent->SetTexture("platformdarkladder.png");
	}
	else
	{
		pRenderComponent->SetTexture("platformdark.png");
	}
	pLevelElementGameObject->AddTag("platform");
	auto pColliderComponent = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
	pColliderComponent->SetShape({ 0,-m_GridElementHeight/6,m_GridElementWidth , m_GridElementHeight/6 });
}

//todo: refactor this
void Level::SpawnPlate(glm::vec2 pos, const std::vector<LevelElement>& levelElements, int row, int col)
{
	auto pScene = GetGameObject()->GetScene();
	GameObject* pLevelElementGameObject{};
	TextureRenderComponent* pRenderComponent{};
	if (col == m_LevelWidth - 1 || levelElements[GetLevelElementsIndex(row, col + 1)].eLevelElement != ELevelElement::plate) //if this is right most element
	{
		//put left and 
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->AddTag("plate");
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("platesingle.png");
		pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
		auto pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
		pBoxCollider->SetShape({ 0,0,m_GridElementWidth, m_GridElementHeight/6 });
	}
	else
	{
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->AddTag("plate");
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("plateleft.png");
		pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
		auto pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
		pBoxCollider->SetShape({ 0,0,m_GridElementWidth, m_GridElementHeight/6 });

		while (!(col == m_LevelWidth - 2 || levelElements[GetLevelElementsIndex(row, col + 2)].eLevelElement != ELevelElement::plate)) //while next element is not right most element
		{
			++col;
			pos.x += m_GridElementWidth;
			pLevelElementGameObject = pScene->CreateAndAddGameObject();
			pLevelElementGameObject->AddTag("plate");
			pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
			pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
			pRenderComponent->SetTexture("platemiddle.png");
			pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
			pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
			pBoxCollider->SetShape({ 0,0,m_GridElementWidth, m_GridElementHeight/6 });
		}

		pos.x += m_GridElementWidth;
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->AddTag("plate");
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("plateright.png");
		pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
		pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
		pBoxCollider->SetShape({ 0,0,m_GridElementWidth, m_GridElementHeight/6 });
	}
}

void Level::SpawnFood(glm::vec2 pos, const std::vector<LevelElement>& levelElements, int row, int col, ELevelElement eLevelElement, const std::string& name)
{
	auto pScene = GetGameObject()->GetScene();
	GameObject* pFoodParentGameObject = pScene->CreateAndAddGameObject("FoodParent");
	pFoodParentGameObject->AddTag("foodparent");
	pFoodParentGameObject->GetTransform()->SetWorldPosition(pos);
	pFoodParentGameObject->CreateAndAddComponent<FoodParent>();
	auto pParentCollider = pFoodParentGameObject->CreateAndAddComponent<BoxCollider>();
	pParentCollider->SetShape({0,0,0,m_GridElementHeight});

	//we know this is left most element
	if (col == m_LevelWidth - 1 || levelElements[GetLevelElementsIndex(row, col + 1)].eLevelElement != eLevelElement) //if this is also right most element
	{
		CreateFoodElement(pos, name + "left1.png", pFoodParentGameObject, pParentCollider);

		pos.x += m_GridElementWidth / 2;
		CreateFoodElement(pos, name + "right2.png", pFoodParentGameObject, pParentCollider);
	}
	else
	{
		CreateFoodElement(pos, name + "left1.png", pFoodParentGameObject, pParentCollider);

		pos.x += m_GridElementWidth / 2;
		CreateFoodElement(pos, name + "left2.png", pFoodParentGameObject, pParentCollider);

		while (!(col == m_LevelWidth - 2 || levelElements[GetLevelElementsIndex(row, col + 2)].eLevelElement != eLevelElement)) //while next element is not right most element
		{
			++col;
			pos.x += m_GridElementWidth / 2;
			CreateFoodElement(pos, name + "mid1.png", pFoodParentGameObject, pParentCollider);

			pos.x += m_GridElementWidth / 2;
			CreateFoodElement(pos, name + "mid2.png", pFoodParentGameObject, pParentCollider);
		}

		pos.x += m_GridElementWidth / 2;
		CreateFoodElement(pos, name + "right1.png", pFoodParentGameObject, pParentCollider);

		pos.x += m_GridElementWidth / 2;
		CreateFoodElement(pos, name + "right2.png", pFoodParentGameObject, pParentCollider);
	}
}

void Level::CreateFoodElement(const glm::vec2 pos, const std::string& textureFileName, GameObject* pParent, BoxCollider* pParentCollider)
{
	auto pScene = GetGameObject()->GetScene();
	auto pLevelElementGameObject = pScene->CreateAndAddGameObject("", pParent);
	pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
	auto pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture(textureFileName);
	pRenderComponent->SetSize({ m_GridElementWidth/2, m_GridElementHeight });
	auto pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
	pBoxCollider->SetShape({ 0,0,m_GridElementWidth / 2, m_GridElementHeight });

	auto newShape = pParentCollider->GetShape();
	newShape.width += m_GridElementWidth / 2;
	pParentCollider->SetShape(newShape);
}

void Level::GenerateNavigableAreas(const std::vector<LevelElement>& levelElements)
{
	m_PlayerNavigableArea.resize(levelElements.size());
	m_EnemyNavigableArea.resize(levelElements.size());

	for (int r{}; r < m_LevelHeight; ++r)
	{
		for (int c{}; c < m_LevelWidth; ++c)
		{
			//build navigation from bottom up, since that's how the level is built, levelelements are from bottom down, since that's how the file was read
			int iLvl = GetLevelElementsIndex(m_LevelHeight - 1 - r, c);
			int iNav = GetLevelElementsIndex(r, c);
			if (levelElements[iLvl].hasLadder)
			{
				m_PlayerNavigableArea[iNav] = true;
				m_EnemyNavigableArea[iNav] = true;
			}
			else if (levelElements[iLvl].eLevelElement != ELevelElement::empty)
			{
				m_EnemyNavigableArea[iNav] = true;
				if (levelElements[iLvl].eLevelElement != ELevelElement::plate && levelElements[iLvl].eLevelElement != ELevelElement::enemyCheat)
				{
					m_PlayerNavigableArea[iNav] = true;
				}
			}
		}
	}
}
