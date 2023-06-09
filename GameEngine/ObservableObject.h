#pragma once
//Problem that I'm trying to solve:
//a component could have a reference to another component or gameobject, for example an ai component has a reference to a player gameobject
//the player gameobject is owned by a scene, the scene should be the only owner, so it stores the gameobject in a unique_ptr
//the ai component needs to know about the player gameobject, so stores it in a raw pointer, since it isn't the owner of the player
//the issue arises when the player gets destroyed, what happens to the raw pointer in the ai component?

//a possible solution would be to use shared_ptrs and in the ai component check if the gameobject has been destroyed, if so, remove the shared_ptr
//now there are no more shared_ptrs to the gameobject, so the gameobject gets destroyed

//but it doesn't make sense since shared_ptr indicated shared ownership, but the ai component isn't the owner of the player gameobject

//so I'm making an observingpointer wrapper class, that needs an observable object
//if the observable object gets destroyed, the observingpointer gets notified and puts its raw pointer to null

//there might be a better way to do this, maybe shared pointers are better after all?
//I couldn't find much help online, most people suggested using shared or weak pointers

//biggest issue here is that almost all classes now derive from observableobject


#include <vector>
#include <list>
#include "Observer.h"

namespace engine
{
	class ObservableObject
	{
	public:
		ObservableObject() = default;
		virtual ~ObservableObject();

		void AddObservingPointer(Observer<>* pObservingPointer) { m_ObservingPointers.emplace_back(pObservingPointer); }
		void RemoveObservingPointer(Observer<>* pObservingPointer) { m_ObservingPointers.erase(std::remove(m_ObservingPointers.begin(), m_ObservingPointers.end(), pObservingPointer)); }

	private:
		std::vector<Observer<>*> m_ObservingPointers{}; //list as container cause adding and removing elements needs to be fast
	};
}

