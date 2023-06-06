#include "ObservableObject.h"
#include "ObservingPointer.h"

engine::ObservableObject::~ObservableObject()
{
	for (auto pObservingPointer : m_ObservingPointers)
	{
		pObservingPointer->Notify();
	}
}
