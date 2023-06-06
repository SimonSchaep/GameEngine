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

//one issue is that almost all classes now derive from observableobject
//there's probably more issues I haven't encountered yet




//todo: write unit tests for this
//todo: complete this class so it can be used by containers


#include "Observer.h"
#include "ObservableObject.h"


namespace engine
{
    //using pimpl here makes it so you can declare an observingpointer with an incomplete type
    template <typename T>
    class ObservingPointerImpl final : public Observer<>
    {
    public:
        ObservingPointerImpl() = default;
        ObservingPointerImpl(T* pObservableObject);
        ~ObservingPointerImpl();

        ObservingPointerImpl(const ObservingPointerImpl& other) = delete;
        ObservingPointerImpl(ObservingPointerImpl&& other) = delete;
        ObservingPointerImpl& operator=(const ObservingPointerImpl& other) = delete;
        ObservingPointerImpl& operator=(ObservingPointerImpl&& other) = delete;

        ObservingPointerImpl<T>& operator=(T* pObservableObject);

        T& operator*() { return *m_RawPointer; }
        T* operator->() { return m_RawPointer; }
        operator bool() const { return m_RawPointer; }

        T* Get()const;

        virtual void Notify();

    private:
        T* m_RawPointer{};
    };

    template <typename T>
    class ObservingPointer final
    {
    public:
        ObservingPointer() : m_Impl(new ObservingPointerImpl<T>()) {}
        ObservingPointer(T* pObservableObject) : m_Impl(new ObservingPointerImpl<T>(pObservableObject)) {}
        ~ObservingPointer() { delete m_Impl; }

        ObservingPointer(const ObservingPointer& other) = delete;
        ObservingPointer(ObservingPointer&& other) = delete;
        ObservingPointer& operator=(const ObservingPointer& other) = delete;
        ObservingPointer& operator=(ObservingPointer&& other) = delete;

        ObservingPointer<T>& operator=(T* pObservableObject)
        {
            *m_Impl = pObservableObject;
            return *this;
        }

        T& operator*() { return **m_Impl; }
        T* operator->() { return m_Impl->operator->(); }
        operator bool() const { return m_Impl->Get(); }

        bool operator==(const ObservingPointer<T>& other)
        {
            return Get() == other.Get();
        }

        bool operator!=(const ObservingPointer<T>& other)
        {
            return !(*this == other);
        }

        T* Get()const { return m_Impl->Get(); }

    private:
        ObservingPointerImpl<T>* m_Impl;
    };

    template <typename T>
    ObservingPointerImpl<T>::ObservingPointerImpl(T* pObservableObject)
        : m_RawPointer(pObservableObject)
    {
        if (m_RawPointer)
        {
            m_RawPointer->AddObservingPointer(this);
        }
    }

    template <typename T>
    ObservingPointerImpl<T>::~ObservingPointerImpl()
    {
        if (m_RawPointer)
        {
            m_RawPointer->RemoveObservingPointer(this);
        }       
    }

    template <typename T>
    ObservingPointerImpl<T>& ObservingPointerImpl<T>::operator=(T* pObservableObject)
    {
        if (m_RawPointer)
        {
            m_RawPointer->RemoveObservingPointer(this);
        }
        m_RawPointer = pObservableObject;
        if (m_RawPointer)
        {
            m_RawPointer->AddObservingPointer(this);
        }
        return *this;
    }

    template <typename T>
    T* ObservingPointerImpl<T>::Get()const
    {
        return m_RawPointer;
    }

    template <typename T>
    void ObservingPointerImpl<T>::Notify()
    {
        m_RawPointer = nullptr;
    }
}





//WIP
// 
//ObservingPointer(const ObservingPointer& other) noexcept
//{
//    m_Impl = new ObservingPointerImpl<T>(other.Get());
//}
//ObservingPointer(ObservingPointer&& other) noexcept
//{
//    m_Impl = new ObservingPointerImpl<T>(std::move(other.Get()));
//}
//ObservingPointer& operator=(const ObservingPointer& other) noexcept
//{
//    if (this != &other)
//    {
//        delete m_Impl;
//        m_Impl = new ObservingPointerImpl<T>(other.Get());
//    }
//    return *this;
//}
//ObservingPointer& operator=(ObservingPointer&& other) noexcept
//{
//    if (this != &other)
//    {
//        delete m_Impl;
//        m_Impl = new ObservingPointerImpl<T>(std::move(other.Get()));
//    }
//    return *this;
//}