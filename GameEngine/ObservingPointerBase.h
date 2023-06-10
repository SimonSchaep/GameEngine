#pragma once
namespace engine
{
	class ObservingPointerBase
	{
	public:
		ObservingPointerBase() = default;
		virtual ~ObservingPointerBase() = default;

		virtual void Notify() = 0;
	};
}