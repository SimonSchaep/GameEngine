#pragma once
#include "ObservableObject.h"

namespace engine
{

	template<typename... Args>
	class Observer : public virtual ObservableObject //is observable, because event needs to have observingpointers to observers, to make sure they are not dangling.
		//Has to be virtual inheritance, since otherwise there might be 2 definitions of observableobject in a component that inherits from observer and basecomponent
		//https://www.sandordargo.com/blog/2020/12/23/virtual-inheritance
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;

		virtual void Notify(Args... args) = 0;
	};

}