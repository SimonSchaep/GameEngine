#pragma once
#include "LevelParser.h"
#include "glm/glm.hpp"

//todo: remove sdl dependance
#include "SDL.h"

namespace LevelBuilder
{
	using namespace LevelParser;

	void BuildLevel(Scene* pScene, const std::string& fileName)
	{
		auto start = std::chrono::high_resolution_clock::now();

		std::vector<LevelElement> levelElements;
		int levelWidth{};
		int levelHeight{};
		ParseCsv("Data/level2.csv", levelElements, levelWidth, levelHeight);

		int screenWidth;
		int screenHeight;
		SDL_GetWindowSize(SDL_GL_GetCurrentWindow(), &screenWidth, &screenHeight);

		std::vector<bool> hasFoodAbove{ std::vector<bool>(levelWidth, false) }; //to make sure platforms below will be dark blue

		float elementWidth{ 32 };
		float elementHeight{ 24 };
		glm::vec2 startPos{ 80,80 };

		for (int r{}; r < levelElements.size() / levelWidth; ++r)
		{
			for (int c{}; c < levelWidth; ++c)
			{
				LevelElement levelElement{ levelElements[r * levelWidth + c] };
				if (levelElement.eLevelElement == ELevelElement::empty && !levelElement.hasLadder) continue;

				glm::vec2 position{ startPos };
				position += glm::vec2{ c * elementWidth, r * elementHeight };
				//std::cout << position.x << ", " << position.y << ", ";

				auto pLevelElementGameObject = pScene->CreateAndAddGameObject();
				pLevelElementGameObject->GetTransform()->SetLocalPosition(position);
				auto pRenderComponent = pLevelElementGameObject->CreateAndAddComponent<TextureRenderComponent>();

				switch (levelElement.eLevelElement)
				{
				case ELevelElement::empty:
					pRenderComponent->SetTexture("ladder.png");
					//std::cout << "ladder\n";
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
					//std::cout << "platform\n";
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
					//std::cout << "topbun\n";
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
					//std::cout << "botbun\n";
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
					//std::cout << "lettuce\n";
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
					//std::cout << "meat\n";
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
					//std::cout << "cheese\n";
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
					//std::cout << "tomato\n";
					break;
				case ELevelElement::bucket:
					hasFoodAbove[c] = true;
					if (levelElement.hasLadder)
					{
						pRenderComponent->SetTexture("platformdarkladder.png");
					}
					else
					{
						pRenderComponent->SetTexture("platformdark.png");
					}
					//std::cout << "bucket\n";
					break;
				}
			}
		}


		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
		std::cout << "Building level took: " << duration << "ms\n";
	}
}

