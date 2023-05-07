#pragma once
#include "Singleton.h"
#include <vector>

//todo: Can be upgraded to physics manager in the future

namespace engine
{
	class Collider;

	class CollisionManager final : public Singleton<CollisionManager>
	{
	public:
		void CheckCollisions();

	private:
		std::vector<Collider*> m_Colliders{};
	};
}