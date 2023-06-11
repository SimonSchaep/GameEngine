#pragma once
#include "ServiceLocator.h"
#include "Logger.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <chrono>

namespace levelParser
{
	class FileNameNotFound : public std::exception
	{
		std::string m_Msg;
	public:
		FileNameNotFound(const std::string& fileName) 
			: m_Msg{ "Level file with path " + fileName + " not found" } {};
		const char* what() const noexcept override
		{
			return m_Msg.c_str();
		}
	};

	class InvalidLevelWidth : public std::exception
	{
		std::string m_Msg;
	public:
		InvalidLevelWidth(int oldLevelWidth, int newLevelWidth)
			:m_Msg{ "Level width " + std::to_string(newLevelWidth) + " is not the same as previous level width " + std::to_string(oldLevelWidth) + ", from the same level file" }{};
		const char* what() const noexcept override
		{
			return m_Msg.c_str();
		}
	};

	class UnknownLevelElement : public std::exception
	{
		std::string m_Msg;
	public:
		UnknownLevelElement(const std::string& element) 
			: m_Msg{ "Level element: " + element + " is unknown" } {};
		const char* what() const noexcept override
		{
			return m_Msg.c_str();
		}
	};

	enum class ELevelElement
	{
		empty,
		platform,		
		topbun,
		botbun,
		lettuce,
		meat,
		cheese,
		tomato,
		plate,
		enemyCheat,
		hotdog,
		egg,
		pickle,
		chef,
	};

	struct LevelElement
	{
		ELevelElement eLevelElement{};
		bool hasLadder{};
	};

	void ParseCsv(const std::string& fileName, std::vector<LevelElement>& levelElements, int& levelWidth, int& levelHeight)
	{
		//auto start = std::chrono::high_resolution_clock::now();

		std::ifstream ifstream{ fileName };

		if (!ifstream.good())
		{
			throw FileNameNotFound(fileName);
			return;
		}

		std::string line{};
		
		levelWidth = 0;
		levelHeight = 0;

		while (std::getline(ifstream, line))
		{
			++levelHeight;
			line.erase(std::remove(line.begin(), line.end(), ' '), line.end()); //remove spaces
			std::stringstream sstream{ line + ','}; //make sure every line ends with a comma
			std::string element{};
			int thisLineElementCount{};
			while (std::getline(sstream, element, ','))
			{
				LevelElement levelElement{};
				std::transform(element.begin(), element.end(), element.begin(), [](char c) { return char(std::toupper(int(c))); });
				if (element != "" && element.back() == 'L') //check for empty string cause then back() doesn't work
				{
					levelElement.hasLadder = true;
					element = element.substr(0, element.size() - 1); //remove 'L'
				}
				if (element == "")
				{
					levelElement.eLevelElement = ELevelElement::empty;
				}
				else if (element == "P")
				{
					levelElement.eLevelElement = ELevelElement::platform;
				}
				else if (element == "TB")
				{
					levelElement.eLevelElement = ELevelElement::topbun;
				}
				else if (element == "BB")
				{
					levelElement.eLevelElement = ELevelElement::botbun;
				}
				else if (element == "LE")
				{
					levelElement.eLevelElement = ELevelElement::lettuce;
				}
				else if (element == "M")
				{
					levelElement.eLevelElement = ELevelElement::meat;
				}
				else if (element == "C")
				{
					levelElement.eLevelElement = ELevelElement::cheese;
				}
				else if (element == "T")
				{
					levelElement.eLevelElement = ELevelElement::tomato;
				}
				else if (element == "PT")
				{
					levelElement.eLevelElement = ELevelElement::plate;
				}
				else if (element == "_")
				{
					levelElement.eLevelElement = ELevelElement::enemyCheat;
				}
				else if (element == "CH")
				{
					levelElement.eLevelElement = ELevelElement::chef;
				}
				else if (element == "HO")
				{
					levelElement.eLevelElement = ELevelElement::hotdog;
				}
				else if (element == "EG")
				{
					levelElement.eLevelElement = ELevelElement::egg;
				}
				else if (element == "PI")
				{
					levelElement.eLevelElement = ELevelElement::pickle;
				}
				else
				{
					throw UnknownLevelElement(element);
				}
				levelElements.emplace_back(levelElement);
				++thisLineElementCount;
			}
			if (levelWidth == 0) levelWidth = thisLineElementCount;
			else if (thisLineElementCount != levelWidth) throw InvalidLevelWidth(thisLineElementCount, levelWidth);
		}


		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
		//engine::ServiceLocator::GetLogger().LogLine("Parsing level file took: " + std::to_string(duration) + "ms", engine::LogType::message);
	}
}