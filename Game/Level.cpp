#include "Level.h"
#include "Scene.h"
#include "LevelParser.h"
#include "TextureRenderComponent.h"

using namespace engine;
using namespace levelParser;

void Level::BuildLevel(Scene* pScene, const std::string& fileName)
{
	auto start = std::chrono::high_resolution_clock::now();

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
			//std::cout << position.x << ", " << position.y << ", ";

			auto pLevelElementGameObject = pScene->CreateAndAddGameObject();
			pLevelElementGameObject->GetTransform()->SetLocalPosition(position);
			auto pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();

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
				break;
			case ELevelElement::plate:
				hasFoodAbove[c] = false;
				if (levelElement.hasLadder)
				{
					std::cout << "Plate shouldn't have ladder\n";
				}
				levelElements[GetLevelElementsIndex(r, c)]
				//pRenderComponent->SetTexture("platemiddle.png");
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
