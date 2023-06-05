#pragma once
#include <vector>
#include <functional>
#include <memory>

namespace engine
{
	class Sprite;
	class SpriteState;

	struct SpriteConnection
	{
		SpriteConnection(SpriteState* to, const std::function<bool()>& predicate) :To{ to }, Predicate{ predicate } {}

		SpriteState* To{};
		std::function<bool()> Predicate{};
	};

	class SpriteState final
	{
	public:
		SpriteState(std::shared_ptr<Sprite> sprite);
		~SpriteState() = default;

		SpriteState(const SpriteState& other) = delete;
		SpriteState(SpriteState&& other) = delete;
		SpriteState& operator=(const SpriteState& other) = delete;
		SpriteState& operator=(SpriteState&& other) = delete;

		std::shared_ptr<Sprite> GetSprite()const { return m_Sprite; }
		void AddConnection(const SpriteConnection& connection) { m_Connections.emplace_back(connection); }

		//returns state that it needs to switch to, nullptr, if no change is needed
		SpriteState* EvaluateConnections();

	private:
		std::vector<SpriteConnection> m_Connections{};
		std::shared_ptr<Sprite> m_Sprite{};
	};
}