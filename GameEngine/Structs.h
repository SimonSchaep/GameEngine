#pragma once
#include "glm/glm.hpp"

namespace engine
{
	namespace structs
	{
		struct Rect
		{
			Rect() = default;
			Rect(const glm::vec2& bottomLeft, float width, float height) :bottomLeft{ bottomLeft }, width{ width }, height{height} {};
			Rect(float x, float y, float width, float height) :bottomLeft{ x,y }, width{ width }, height{height} {};
			glm::vec2 bottomLeft;
			float width;
			float height;

			//this auto generates all comparison operators
			//https://en.cppreference.com/w/cpp/language/operators
			auto operator<=>(const Rect&) const = default;
		};

		struct Circle
		{
			Circle() = default;
			Circle(const glm::vec2& center, float radius) :center{ center }, radius{ radius } {};
			Circle(float x, float y, float radius) :center{ x,y }, radius{ radius } {};
			glm::vec2 center;
			float radius;

			//this auto generates all comparison operators
			//https://en.cppreference.com/w/cpp/language/operators
			auto operator<=>(const Circle&) const = default;
		};
	}
}