#pragma once
#include <string>
#include <functional>

namespace dae
{
	class GameEngine
	{
	public:
		explicit GameEngine(const std::string& dataPath);
		~GameEngine();
		void Run(const std::function<void()>& load);

		GameEngine(const GameEngine& other) = delete;
		GameEngine(GameEngine&& other) = delete;
		GameEngine& operator=(const GameEngine& other) = delete;
		GameEngine& operator=(GameEngine&& other) = delete;
	};
}