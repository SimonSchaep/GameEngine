#pragma once
#include "glm/glm.hpp"

namespace engine
{
	namespace structs
	{
		struct Rect
		{
			Rect() = default;
			Rect(const glm::vec2& bottomLeft, float width, float height) :BottomLeft{ bottomLeft }, Width{ width }, Height{height} {};
			Rect(float x, float y, float width, float height) :BottomLeft{ x,y }, Width{ width }, Height{height} {};
			glm::vec2 BottomLeft;
			float Width;
			float Height;
		};

		struct Circle
		{
			Circle() = default;
			Circle(const glm::vec2& center, float radius) :Center{ center }, Radius{ radius } {};
			Circle(float x, float y, float radius) :Center{ x,y }, Radius{ radius } {};
			glm::vec2 Center;
			float Radius;
		};
	}
}