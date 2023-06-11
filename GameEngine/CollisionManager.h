#pragma once
#include "Singleton.h"

//only does triggers for now
//todo: Can be upgraded to physics manager in the future

namespace engine
{
	class Collider;

	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void CheckTriggers();

		void Render()const;
	private:
		friend class Singleton<CollisionManager>;
		CollisionManager() = default;
	};
}