#include "Level.h"
#include "Scene.h"
#include "LevelParser.h"
#include "TextureRenderComponent.h"
#include "BoxCollider.h"
#include "ColliderDebugRenderComponent.h"
#include "FoodParent.h"
#include "ServiceLocator.h"
#include "Logger.h"
#include "ChefLogic.h"
#include "ThrowPepperComponent.h"
#include "MovementComponent.h"
#include "SpriteRenderComponent.h"
#include "ChefSpriteController.h"
#include "EnemyLogic.h"
#include "PickleSpriteController.h"
#include "HotdogSpriteController.h"
#include "EggSpriteController.h"
#include "LayersEnum.h"
#include "CameraComponent.h"

using namespace engine;
using namespace levelParser;

Level::Level(engine::GameObject* pGameObject, const std::string& levelFileName)
	:BaseComponent(pGameObject), m_LevelFileName{ levelFileName }
{
}

Level::~Level() = default;

void Level::Initialize()
{
}

bool Level::IsNavigable(int row, int col, bool isEnemy) const
{
	if (col < 0 || col > m_LevelWidth - 1 || row < 0 || row > m_LevelHeight - 1)
	{
		return false;
	}

	if (isEnemy)
	{
		return m_EnemyNavigableArea[col + row * m_LevelWidth];
	}
	else
	{
		return m_ChefNavigableArea[col + row * m_LevelWidth];
	}
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

int Level::GetIndexOfPos(const glm::vec2& pos) const
{
	int row, col;
	GetRowColOfPos(pos, row, col);
	return GetIndexFromRowCol(row, col);
}

glm::vec2 Level::GetCenterOfCell(int row, int col) const
{
	return m_LevelStartPos + glm::vec2{(col + 0.5f) * m_GridElementWidth, (row + 0.5f) * m_GridElementHeight };
}

glm::vec2 Level::GetCenterOfCell(int index) const
{
	int row, col;
	GetRowColOfIndex(index, row, col);
	return GetCenterOfCell(row, col);
}

engine::GameObject* Level::SpawnChef(glm::vec2 pos, bool isAlreadyCentered)
{
	const float width{ 28 };
	const float height{ 28 };
	const float visualsHeightOffset{ -height / 3.5f };
	
	if (!isAlreadyCentered)
	{
		pos += glm::vec2{m_GridElementWidth - width, m_GridElementHeight - height};
		pos.x += m_GridElementWidth / 2 - (m_GridElementWidth - width);
		pos.y += m_GridElementHeight / 2 - (m_GridElementHeight - height);
	}

	auto pChef = GetScene()->CreateAndAddGameObject("Chef");
	pChef->AddTag("Chef");
	pChef->GetTransform()->SetLocalPosition(pos);
	pChef->CreateAndAddComponent<ThrowPepperComponent>();
	auto pMovementComponent = pChef->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(m_ChefSpeed);
	pChef->CreateAndAddComponent<ChefLogic>();

	//visuals
	auto pChefVisuals = GetScene()->CreateAndAddGameObject("ChefVisuals", pChef);
	auto pSpriteRenderComponent = pChefVisuals->CreateAndAddComponent<SpriteRenderComponent>();
	pSpriteRenderComponent->SetSize({ width, height });
	pSpriteRenderComponent->SetLayer(Layer::chef);

	pChefVisuals->CreateAndAddComponent<ChefSpriteController>();

	pChefVisuals->GetTransform()->SetLocalPosition({ -width / 2, visualsHeightOffset });

	//collider
	auto pBoxCollider = pChef->CreateAndAddComponent<BoxCollider>();
	pBoxCollider->SetShape({ -width / 4, visualsHeightOffset, width / 2, height / 4 });

	//camera
	//camera is not set up to work correctly in this game
	//cause it can only follow 1 player
	//uncomment the following lines if you want to see the camera in action
	/*auto pCamera = pChef->CreateAndAddComponent<CameraComponent>();
	GetScene()->SetActiveCamera(pCamera);
	pCamera->SetLevelBoundaries({ GetLevelPosition().x,GetLevelPosition().y,float(GetLevelWidth() * GetCellWidth()),float((GetLevelHeight() + 1) * GetCellHeight()) });*/

	return pChef;
}

void Level::SpawnHotdog(engine::TextureRenderComponent* pRenderComponent, engine::GameObject* pLevelElementGameObject, glm::vec2 pos)
{
	const float width{ 28 };
	const float height{ 28 };
	const float visualsHeightOffset{ -height / 3.5f };

	pos += glm::vec2{m_GridElementWidth - width, m_GridElementHeight - height};
	pos.x += m_GridElementWidth / 2 - (m_GridElementWidth - width);
	pos.y += m_GridElementHeight / 2 - (m_GridElementHeight - height);

	auto pHotdog = GetScene()->CreateAndAddGameObject("Hotdog");
	pHotdog->AddTag("Enemy");
	pHotdog->GetTransform()->SetLocalPosition(pos);
	auto pMovementComponent = pHotdog->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(m_EnemySpeed);
	pHotdog->CreateAndAddComponent<EnemyLogic>();

	//visuals
	auto pHotdogVisuals = GetScene()->CreateAndAddGameObject("HotdogVisuals", pHotdog);
	auto pSpriteRenderComponent = pHotdogVisuals->CreateAndAddComponent<SpriteRenderComponent>();
	pSpriteRenderComponent->SetSize({ width, height });
	pSpriteRenderComponent->SetLayer(Layer::enemy);

	pHotdogVisuals->CreateAndAddComponent<HotdogSpriteController>();

	pHotdogVisuals->GetTransform()->SetLocalPosition({ -width / 2, visualsHeightOffset });

	//black cover
	pLevelElementGameObject->GetTransform()->SetWorldPosition(pHotdogVisuals->GetTransform()->GetWorldPosition());

	pRenderComponent->SetTexture("black.png");
	pRenderComponent->SetLayer(Layer::enemyCover);
	pRenderComponent->SetSize({ width, height });

	//collider
	auto pBoxCollider = pHotdog->CreateAndAddComponent<BoxCollider>();
	pBoxCollider->SetShape({ -width / 4, visualsHeightOffset, width / 2, height / 4 });
}

void Level::SpawnEgg(engine::TextureRenderComponent* pRenderComponent, engine::GameObject* pLevelElementGameObject, glm::vec2 pos)
{
	const float width{ 28 };
	const float height{ 28 };
	const float visualsHeightOffset{ -height / 3.5f };

	pos += glm::vec2{m_GridElementWidth - width, m_GridElementHeight - height};
	pos.x += m_GridElementWidth / 2 - (m_GridElementWidth - width);
	pos.y += m_GridElementHeight / 2 - (m_GridElementHeight - height);

	auto pEgg = GetScene()->CreateAndAddGameObject("Egg");
	pEgg->AddTag("Enemy");
	pEgg->GetTransform()->SetLocalPosition(pos);
	auto pMovementComponent = pEgg->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(m_EnemySpeed);
	pEgg->CreateAndAddComponent<EnemyLogic>();

	//visuals
	auto pEggVisuals = GetScene()->CreateAndAddGameObject("EggVisuals", pEgg);
	auto pSpriteRenderComponent = pEggVisuals->CreateAndAddComponent<SpriteRenderComponent>();
	pSpriteRenderComponent->SetSize({ width, height });
	pSpriteRenderComponent->SetLayer(Layer::enemy);

	pEggVisuals->CreateAndAddComponent<EggSpriteController>();

	pEggVisuals->GetTransform()->SetLocalPosition({ -width / 2, visualsHeightOffset });

	//black cover
	pLevelElementGameObject->GetTransform()->SetWorldPosition(pEggVisuals->GetTransform()->GetWorldPosition());

	pRenderComponent->SetTexture("black.png");
	pRenderComponent->SetLayer(Layer::enemyCover);
	pRenderComponent->SetSize({ width, height });

	//collider
	auto pBoxCollider = pEgg->CreateAndAddComponent<BoxCollider>();
	pBoxCollider->SetShape({ -width / 4, visualsHeightOffset, width / 2, height / 4 });
}

void Level::SpawnPickle(engine::TextureRenderComponent* pRenderComponent, engine::GameObject* pLevelElementGameObject, glm::vec2 pos)
{
	const float width{ 28 };
	const float height{ 28 };
	const float visualsHeightOffset{ -height / 3.5f };

	pos += glm::vec2{m_GridElementWidth - width, m_GridElementHeight - height};
	pos.x += m_GridElementWidth / 2 - (m_GridElementWidth - width);
	pos.y += m_GridElementHeight / 2 - (m_GridElementHeight - height);

	auto pPickle = GetScene()->CreateAndAddGameObject("Pickle");
	pPickle->AddTag("Enemy");
	pPickle->GetTransform()->SetLocalPosition(pos);
	auto pMovementComponent = pPickle->CreateAndAddComponent<MovementComponent>();
	pMovementComponent->SetMoveSpeed(m_EnemySpeed);
	pPickle->CreateAndAddComponent<EnemyLogic>();

	//visuals
	auto pPickleVisuals = GetScene()->CreateAndAddGameObject("PickleVisuals", pPickle);
	auto pSpriteRenderComponent = pPickleVisuals->CreateAndAddComponent<SpriteRenderComponent>();
	pSpriteRenderComponent->SetSize({ width, height });
	pSpriteRenderComponent->SetLayer(Layer::enemy);

	pPickleVisuals->CreateAndAddComponent<PickleSpriteController>();

	pPickleVisuals->GetTransform()->SetLocalPosition({ -width / 2, visualsHeightOffset });

	//black cover
	pLevelElementGameObject->GetTransform()->SetWorldPosition(pPickleVisuals->GetTransform()->GetWorldPosition());

	pRenderComponent->SetTexture("black.png");
	pRenderComponent->SetLayer(Layer::enemyCover);
	pRenderComponent->SetSize({ width, height });

	//collider
	auto pBoxCollider = pPickle->CreateAndAddComponent<BoxCollider>();
	pBoxCollider->SetShape({ -width / 4, visualsHeightOffset, width / 2, height / 4 });
}

void Level::BuildLevel()
{
	//auto start = std::chrono::high_resolution_clock::now();

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
			LevelElement levelElement{ levelElements[GetIndexFromRowCol(r, c)] };

			if ((levelElement.eLevelElement == ELevelElement::empty && !levelElement.hasLadder) || levelElement.eLevelElement == ELevelElement::enemyCheat) continue;

			glm::vec2 position{ m_LevelStartPos };
			position += glm::vec2{ c* m_GridElementWidth, (m_LevelHeight - 1 - r)* m_GridElementHeight };

			GameObject* pLevelElementGameObject{};
			TextureRenderComponent* pRenderComponent{};
			if (levelElement.eLevelElement != ELevelElement::plate) //plate doesn't have platform behind it
			{
				pLevelElementGameObject = pScene->CreateAndAddGameObject();
				pLevelElementGameObject->GetTransform()->SetLocalPosition(position);
				pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
				pRenderComponent->SetLayer(Layer::level);
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

				if (c == 0 || levelElements[GetIndexFromRowCol(r, c - 1)].eLevelElement != ELevelElement::topbun) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::topbun, "topbun");
				}
				break;
			case ELevelElement::botbun:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetIndexFromRowCol(r, c - 1)].eLevelElement != ELevelElement::botbun) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::botbun, "botbun");
				}
				break;
			case ELevelElement::lettuce:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetIndexFromRowCol(r, c - 1)].eLevelElement != ELevelElement::lettuce) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::lettuce, "lettuce");
				}
				break;
			case ELevelElement::meat:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetIndexFromRowCol(r, c - 1)].eLevelElement != ELevelElement::meat) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::meat, "meat");
				}
				break;
			case ELevelElement::cheese:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetIndexFromRowCol(r, c - 1)].eLevelElement != ELevelElement::cheese) //if this is left most element
				{
					SpawnFood(position + foodOffset, levelElements, r, c, ELevelElement::cheese, "cheese");
				}
				break;
			case ELevelElement::tomato:
				hasFoodAbove[c] = true;
				CreateDarkPlatform(pRenderComponent, pLevelElementGameObject, levelElement.hasLadder);

				if (c == 0 || levelElements[GetIndexFromRowCol(r, c - 1)].eLevelElement != ELevelElement::tomato) //if this is left most element
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
				if (c == 0 || levelElements[GetIndexFromRowCol(r, c - 1)].eLevelElement != ELevelElement::plate) //if this is left most element
				{
					SpawnPlate(position, levelElements, r, c);
				}
				break;
			case ELevelElement::chef:
				//spawn as if it was platform
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
				//also spawn a chef
				SpawnChef(position, false);
				break;
			case ELevelElement::hotdog:
				SpawnHotdog(pRenderComponent, pLevelElementGameObject, position);
				break;
			case ELevelElement::egg:
				SpawnEgg(pRenderComponent, pLevelElementGameObject, position);
				break;
			case ELevelElement::pickle:
				SpawnPickle(pRenderComponent, pLevelElementGameObject,position);
				break;
			}
		}
	}

	GenerateNavigableAreas(levelElements);

	//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
	//ServiceLocator::GetLogger().LogLine(("Building level took: " + std::to_string(duration) + "ms"), LogType::message);
}

int Level::GetIndexFromRowCol(int row, int col)const
{
	return row * m_LevelWidth + col;
}

void Level::GetRowColOfIndex(int index, int& row, int& col) const
{
	row = index / m_LevelWidth;
	col = index % m_LevelWidth;
}

std::vector<int> Level::GetAdjacentNavigableTiles(int index, bool includeCheats) const
{
	std::vector<int> adjacentTiles{};
	int row, col;
	GetRowColOfIndex(index, row, col);
	//Right
	if (IsNavigable(row, col + 1, includeCheats))
	{
		adjacentTiles.emplace_back(GetIndexFromRowCol(row, col + 1));
	}
	//Left
	if (IsNavigable(row, col - 1, includeCheats))
	{
		adjacentTiles.emplace_back(GetIndexFromRowCol(row, col - 1));
	}
	//Up
	if (IsNavigable(row + 1, col, includeCheats))
	{
		adjacentTiles.emplace_back(GetIndexFromRowCol(row + 1, col));
	}
	//Down
	if (IsNavigable(row - 1, col, includeCheats))
	{
		adjacentTiles.emplace_back(GetIndexFromRowCol(row - 1, col));
	}
	return adjacentTiles;
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
	if (col == m_LevelWidth - 1 || levelElements[GetIndexFromRowCol(row, col + 1)].eLevelElement != ELevelElement::plate) //if this is right most element
	{
		//put left and 
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->AddTag("plate");
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("platesingle.png");
		pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
		pRenderComponent->SetLayer(Layer::plate);
		auto pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
		pBoxCollider->SetShape({ 0,0,m_GridElementWidth, m_GridElementHeight * 0.2f });
	}
	else
	{
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->AddTag("plate");
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("plateleft.png");
		pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
		pRenderComponent->SetLayer(Layer::plate);
		auto pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
		pBoxCollider->SetShape({ 0,0,m_GridElementWidth, m_GridElementHeight * 0.2f });

		while (!(col == m_LevelWidth - 2 || levelElements[GetIndexFromRowCol(row, col + 2)].eLevelElement != ELevelElement::plate)) //while next element is not right most element
		{
			++col;
			pos.x += m_GridElementWidth;
			pLevelElementGameObject = pScene->CreateAndAddGameObject();
			pLevelElementGameObject->AddTag("plate");
			pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
			pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
			pRenderComponent->SetTexture("platemiddle.png");
			pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
			pRenderComponent->SetLayer(Layer::plate);
			pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
			pBoxCollider->SetShape({ 0,0,m_GridElementWidth, m_GridElementHeight * 0.2f });
		}

		pos.x += m_GridElementWidth;
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->AddTag("plate");
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("plateright.png");
		pRenderComponent->SetSize({ m_GridElementWidth, m_GridElementHeight });
		pRenderComponent->SetLayer(Layer::plate);
		pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
		pBoxCollider->SetShape({ 0,0,m_GridElementWidth, m_GridElementHeight*0.2f });
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
	if (col == m_LevelWidth - 1 || levelElements[GetIndexFromRowCol(row, col + 1)].eLevelElement != eLevelElement) //if this is also right most element
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

		while (!(col == m_LevelWidth - 2 || levelElements[GetIndexFromRowCol(row, col + 2)].eLevelElement != eLevelElement)) //while next element is not right most element
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

void Level::CreateFoodElement(const glm::vec2& pos, const std::string& textureFileName, GameObject* pParent, BoxCollider* pParentCollider)
{
	auto pScene = GetGameObject()->GetScene();
	auto pLevelElementGameObject = pScene->CreateAndAddGameObject("", pParent);
	pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
	auto pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
	pRenderComponent->SetTexture(textureFileName);
	pRenderComponent->SetSize({ m_GridElementWidth/2, m_GridElementHeight });
	pRenderComponent->SetLayer(Layer::food);
	auto pBoxCollider = pLevelElementGameObject->CreateAndAddComponent<BoxCollider>();
	pBoxCollider->SetShape({ 0,0,m_GridElementWidth / 2, m_GridElementHeight });

	auto newShape = pParentCollider->GetShape();
	newShape.width += m_GridElementWidth / 2;
	pParentCollider->SetShape(newShape);
}

void Level::GenerateNavigableAreas(const std::vector<LevelElement>& levelElements)
{
	m_ChefNavigableArea.resize(levelElements.size());
	m_EnemyNavigableArea.resize(levelElements.size());

	for (int r{}; r < m_LevelHeight; ++r)
	{
		for (int c{}; c < m_LevelWidth; ++c)
		{
			//build navigation from bottom up, since that's how the level is built, levelelements are from bottom down, since that's how the file was read
			int iLvl = GetIndexFromRowCol(m_LevelHeight - 1 - r, c);
			int iNav = GetIndexFromRowCol(r, c);
			if (levelElements[iLvl].hasLadder)
			{
				m_ChefNavigableArea[iNav] = true;
				m_EnemyNavigableArea[iNav] = true;
			}
			else if (levelElements[iLvl].eLevelElement != ELevelElement::empty)
			{
				m_EnemyNavigableArea[iNav] = true;
				if (levelElements[iLvl].eLevelElement != ELevelElement::plate && levelElements[iLvl].eLevelElement != ELevelElement::enemyCheat
					&& levelElements[iLvl].eLevelElement != ELevelElement::hotdog && levelElements[iLvl].eLevelElement != ELevelElement::egg
					&& levelElements[iLvl].eLevelElement != ELevelElement::pickle)
				{
					m_ChefNavigableArea[iNav] = true;
				}
			}
		}
	}
}
