#include "Level.h"
#include "Scene.h"
#include "LevelParser.h"
#include "TextureRenderComponent.h"

using namespace engine;
using namespace levelParser;

void Level::BuildLevel(Scene* pScene, const std::string& fileName)
{
	auto start = std::chrono::high_resolution_clock::now();

	const glm::vec2 foodOffset{0,2};

	std::vector<LevelElement> levelElements;
	
	try
	{
		ParseCsv(fileName, levelElements, m_LevelWidth, m_LevelHeight);
	}
	catch (levelParser::FileNameNotFound& except)
	{
		std::cout << except.what();
		return;
	}
	catch (levelParser::InvalidLevelWidth& except)
	{
		std::cout << except.what();
		return;
	}
	catch (levelParser::UnknownLevelElement& except)
	{
		std::cout << except.what();
		return;
	}

	std::vector<bool> hasFoodAbove{ std::vector<bool>(m_LevelWidth, false) }; //to make sure platforms below will be dark blue

	for (int r{}; r < levelElements.size() / m_LevelWidth; ++r)
	{
		for (int c{}; c < m_LevelWidth; ++c)
		{
			LevelElement levelElement{ levelElements[GetLevelElementsIndex(r, c)] };

			if (levelElement.eLevelElement == ELevelElement::empty && !levelElement.hasLadder) continue;

			glm::vec2 position{ m_LevelStartPos };
			position += glm::vec2{ c * m_GridElementWidth, r * m_GridElementHeight };

			GameObject* pLevelElementGameObject{};
			TextureRenderComponent* pRenderComponent{};
			if (levelElement.eLevelElement != ELevelElement::plate) //plate doesn't have gameobject underneath it
			{
				pLevelElementGameObject = pScene->CreateAndAddGameObject();
				pLevelElementGameObject->GetTransform()->SetLocalPosition(position);
				pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
				pRenderComponent->SetLayer(-1);
			}

			switch (levelElement.eLevelElement)
			{
			case ELevelElement::empty:
				pRenderComponent->SetTexture("ladder.png");
				break;
			case ELevelElement::platform:
				if (hasFoodAbove[c])
				{
					if (levelElement.hasLadder)
					{
						pRenderComponent->SetTexture("platformdarkladder.png");
					}
					else
					{
						pRenderComponent->SetTexture("platformdark.png");
					}
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
				if (levelElement.hasLadder)
				{
					pRenderComponent->SetTexture("platformdarkladder.png");
				}
				else
				{
					pRenderComponent->SetTexture("platformdark.png");
				}
				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::topbun) //if this is left most element
				{
					SpawnFood(position + foodOffset, pScene, levelElements, r, c, ELevelElement::topbun, "topbun");
				}
				break;
			case ELevelElement::botbun:
				hasFoodAbove[c] = true;
				if (levelElement.hasLadder)
				{
					pRenderComponent->SetTexture("platformdarkladder.png");
				}
				else
				{
					pRenderComponent->SetTexture("platformdark.png");
				}
				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::botbun) //if this is left most element
				{
					SpawnFood(position + foodOffset, pScene, levelElements, r, c, ELevelElement::botbun, "botbun");
				}
				break;
			case ELevelElement::lettuce:
				hasFoodAbove[c] = true;
				if (levelElement.hasLadder)
				{
					pRenderComponent->SetTexture("platformdarkladder.png");
				}
				else
				{
					pRenderComponent->SetTexture("platformdark.png");
				}
				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::lettuce) //if this is left most element
				{
					SpawnFood(position + foodOffset, pScene, levelElements, r, c, ELevelElement::lettuce, "lettuce");
				}
				break;
			case ELevelElement::meat:
				hasFoodAbove[c] = true;
				if (levelElement.hasLadder)
				{
					pRenderComponent->SetTexture("platformdarkladder.png");
				}
				else
				{
					pRenderComponent->SetTexture("platformdark.png");
				}
				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::meat) //if this is left most element
				{
					SpawnFood(position + foodOffset, pScene, levelElements, r, c, ELevelElement::meat, "meat");
				}
				break;
			case ELevelElement::cheese:
				hasFoodAbove[c] = true;
				if (levelElement.hasLadder)
				{
					pRenderComponent->SetTexture("platformdarkladder.png");
				}
				else
				{
					pRenderComponent->SetTexture("platformdark.png");
				}
				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::cheese) //if this is left most element
				{
					SpawnFood(position + foodOffset, pScene, levelElements, r, c, ELevelElement::cheese, "cheese");
				}
				break;
			case ELevelElement::tomato:
				hasFoodAbove[c] = true;
				if (levelElement.hasLadder)
				{
					pRenderComponent->SetTexture("platformdarkladder.png");
				}
				else
				{
					pRenderComponent->SetTexture("platformdark.png");
				}
				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::tomato) //if this is left most element
				{
					SpawnFood(position + foodOffset, pScene, levelElements, r, c, ELevelElement::tomato, "tomato");
				}
				break;
			case ELevelElement::plate:
				hasFoodAbove[c] = false;
				if (levelElement.hasLadder)
				{
					std::cout << "Plate shouldn't have ladder\n";
				}
				if (c == 0 || levelElements[GetLevelElementsIndex(r, c - 1)].eLevelElement != ELevelElement::plate) //if this is left most element
				{
					SpawnPlate(position, pScene, levelElements, r, c);
				}
				break;
			}
		}
	}

	GenerateNavigableAreas(levelElements);

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
	std::cout << "Building level took: " << duration << "ms\n";
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

int Level::GetLevelElementsIndex(int row, int col)
{
	return row * m_LevelWidth + col;
}

void Level::SpawnPlate(glm::vec2 pos, Scene* pScene, const std::vector<LevelElement>& levelElements, int row, int col)
{

	GameObject* pLevelElementGameObject{};
	TextureRenderComponent* pRenderComponent{};
	if (col == m_LevelWidth - 1 || levelElements[GetLevelElementsIndex(row, col + 1)].eLevelElement != ELevelElement::plate) //if this is right most element
	{
		//put left and 
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("platesingle.png");
	}
	else
	{
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("plateleft.png");

		while (!(col == m_LevelWidth - 2 || levelElements[GetLevelElementsIndex(row, col + 2)].eLevelElement != ELevelElement::plate)) //while next element is not right most element
		{
			++col;
			pos.x += m_GridElementWidth;
			pLevelElementGameObject = pScene->CreateAndAddGameObject();
			pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
			pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
			pRenderComponent->SetTexture("platemiddle.png");
		}

		pos.x += m_GridElementWidth;
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture("plateright.png");
	}
}

void Level::SpawnFood(glm::vec2 pos, Scene* pScene, const std::vector<LevelElement>& levelElements, int row, int col, ELevelElement eLevelElement, const std::string& name)
{
	GameObject* pLevelElementGameObject{};
	TextureRenderComponent* pRenderComponent{};
	if (col == m_LevelWidth - 1 || levelElements[GetLevelElementsIndex(row, col + 1)].eLevelElement != eLevelElement) //if this is right most element
	{
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture(name + "left1.png");

		pos.x += m_GridElementWidth / 2;
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture(name + "right2.png");
	}
	else
	{
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture(name + "left1.png");

		pos.x += m_GridElementWidth / 2;
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture(name + "left2.png");

		while (!(col == m_LevelWidth - 2 || levelElements[GetLevelElementsIndex(row, col + 2)].eLevelElement != eLevelElement)) //while next element is not right most element
		{
			++col;
			pos.x += m_GridElementWidth / 2;
			pLevelElementGameObject = pScene->CreateAndAddGameObject();
			pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
			pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
			pRenderComponent->SetTexture(name + "mid1.png");

			pos.x += m_GridElementWidth / 2;
			pLevelElementGameObject = pScene->CreateAndAddGameObject();
			pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
			pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
			pRenderComponent->SetTexture(name + "mid2.png");
		}

		pos.x += m_GridElementWidth / 2;
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture(name + "right1.png");

		pos.x += m_GridElementWidth / 2;
		pLevelElementGameObject = pScene->CreateAndAddGameObject();
		pLevelElementGameObject->GetTransform()->SetWorldPosition(pos);
		pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();
		pRenderComponent->SetTexture(name + "right2.png");
	}
}

void Level::GenerateNavigableAreas(const std::vector<LevelElement>& levelElements)
{
	m_PlayerNavigableArea.resize(levelElements.size());
	m_EnemyNavigableArea.resize(levelElements.size());

	for (size_t i{}; i < levelElements.size(); ++i)
	{
		if (levelElements[i].hasLadder)
		{
			m_PlayerNavigableArea[i] = true;
			m_EnemyNavigableArea[i] = true;
		}
		else if(levelElements[i].eLevelElement != ELevelElement::empty)
		{
			m_EnemyNavigableArea[i] = true;
			if (levelElements[i].eLevelElement != ELevelElement::plate && levelElements[i].eLevelElement != ELevelElement::enemyCheat)
			{
				m_PlayerNavigableArea[i] = true;
			}
		}
	}
}
