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
		if (connection.predicate())
		{
			return connection.to;
		}
	}

	return nullptr; //means no state change needed
}
