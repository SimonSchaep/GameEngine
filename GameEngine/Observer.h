#pragma once
namespace engine
{

	template<typename... Args>
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;

		virtual void Notify(Args... args) = 0;
	};

}