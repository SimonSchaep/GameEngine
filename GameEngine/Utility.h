#pragma once
#include <glm/glm.hpp>

namespace engine
{
	namespace utility
	{
		//no distance squared in glm as far as I know
		template<typename T>
		float DistanceSquared(const T& vec1, const T& vec2)
		{
			return (vec2.x - vec1.x) * (vec2.x - vec1.x) + (vec2.y - vec1.y) * (vec2.y - vec1.y);
		}
	}
}