#include "SpriteState.h"
#include "Sprite.h"

engine::SpriteState::SpriteState(std::shared_ptr<Sprite> sprite)
	:m_Sprite{ std::move(sprite) }
{
}

engine::SpriteState* engine::SpriteState::EvaluateConnections()
{
	for (const SpriteConnection& connection : m_Connections)
	{
		if (connection.Predicate()) //todo: not sure if this will work, test
		{
			return connection.To;
		}
	}

	return nullptr; //no change needed
}
