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
//todo: right now adding and removing is expensive, cause there is an empla


#include "Observer.h"
#include "ObservableObject.h"
#include <type_traits>


namespace engine
{
    //using pimpl here makes it so you can declare an observingpointer with an incomplete type
    template <typename T>
    class ObservingPointer final : public Observer<>
    {
    public:
        ObservingPointer() = default;
        ObservingPointer(T* pObservableObject);
        ~ObservingPointer();

        ObservingPointer(const ObservingPointer& other) noexcept;
        ObservingPointer(ObservingPointer&& other) noexcept;
        ObservingPointer& operator=(const ObservingPointer& other) noexcept;
        ObservingPointer& operator=(ObservingPointer&& other) noexcept;

        ObservingPointer<T>& operator=(T* pObservableObject);

        T& operator*() { return *m_RawPointer; }
        T* operator->() { return m_RawPointer; }
        operator bool() const { return m_RawPointer; }

        T* Get()const;

        virtual void Notify();

    private:
        T* m_RawPointer{};
    };

    // Chatgpt wrote this when I asked how to make this template class work with undefined types as template parameters
    // I don't understand 100% of it
    // but it comes down to this:
    // without the constexpr check in the destructor, the compiler needs to know if T can do RemoveObservingPointer
    // so when T is undefined, it can't know for sure
    // to fix this
    // we add a compile-time check with constexpr
    // if T has checkRemoveObservingPointer(), it will take the first overload and return true_type
    // otherwise it will take the second overload and return false_type
    // But then why does this check not need a defined type?
    // I'm still unsure why this check is only needed in the destructor
    // maybe because the compiler only needs to know about the destructor when a variable is declared?
    namespace checks
    {
        template <typename T>
        auto checkRemoveObservingPointer(T* pObservableObject)
            -> decltype(pObservableObject->RemoveObservingPointer(std::declval<ObservingPointer<T>*>()), std::true_type{});

        template <typename T>
        auto checkRemoveObservingPointer(...)
            -> std::false_type;
    }

    template <typename T>
    ObservingPointer<T>::ObservingPointer(T* pObservableObject)
        : m_RawPointer(pObservableObject)
    {
        if (m_RawPointer)
        {
            m_RawPointer->AddObservingPointer(this);
        }
    }

    template <typename T>
    ObservingPointer<T>::~ObservingPointer()
    {
        if constexpr (decltype(checks::checkRemoveObservingPointer<T>(nullptr))::value)
        {
            if (m_RawPointer)
            {
                m_RawPointer->RemoveObservingPointer(this);
            }
        }
    }

    template<typename T>
    ObservingPointer<T>::ObservingPointer(ObservingPointer&& other) noexcept
        :m_RawPointer{std::move(other.Get())}
    {
        if (m_RawPointer)
        {
            m_RawPointer->AddObservingPointer(this);
        }
    }

    template<typename T>
    ObservingPointer<T>::ObservingPointer(const ObservingPointer& other) noexcept
        :m_RawPointer{ other.Get() }
    {
        if (m_RawPointer)
        {
            m_RawPointer->AddObservingPointer(this);
        }
    }

    template<typename T>
    ObservingPointer<T>& ObservingPointer<T>::operator=(ObservingPointer<T>&& other) noexcept
    {
        *this = std::move(other.Get());
        return *this;
    }

    template<typename T>
    ObservingPointer<T>& ObservingPointer<T>::operator=(const ObservingPointer<T>& other) noexcept
    {
        *this = other.Get();
        return *this;
    }

    template <typename T>
    ObservingPointer<T>& ObservingPointer<T>::operator=(T* pObservableObject)
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
    T* ObservingPointer<T>::Get()const
    {
        return m_RawPointer;
    }

    template <typename T>
    void ObservingPointer<T>::Notify()
    {
        m_RawPointer = nullptr;
    }

    //Comparison operators, based on: https://en.cppreference.com/w/cpp/memory/shared_ptr/operator_cmp
    template<typename T, typename U>
    bool operator==(const ObservingPointer<T>& lhs, const ObservingPointer<U>& rhs) noexcept
    {
        return lhs.Get() == rhs.Get();
    }
    template<typename T, typename U>
    bool operator!=(const ObservingPointer<T>& lhs, const ObservingPointer<U>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template<typename T>
    bool operator==(const ObservingPointer<T>& lhs, const T* rhs) noexcept
    {
        return lhs.Get() == rhs;
    }
    template<typename T>
    bool operator!=(const ObservingPointer<T>& lhs, const T* rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template<typename T>
    bool operator==(const T* lhs, const ObservingPointer<T>& rhs) noexcept
    {
        return lhs == rhs.Get();
    }
    template<typename T>
    bool operator!=(const T* lhs, const ObservingPointer<T>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template<typename T>
    bool operator==(const ObservingPointer<T>& lhs, std::nullptr_t) noexcept
    {
        return !lhs;
    }
    template<typename T>
    bool operator==(std::nullptr_t, const ObservingPointer<T>& rhs) noexcept
    {
        return !rhs;
    }
    template<typename T>
    bool operator!=(const ObservingPointer<T>& lhs, std::nullptr_t) noexcept
    {
        return (bool)lhs;
    }
    template<typename T>
    bool operator!=(std::nullptr_t, const ObservingPointer<T>& rhs) noexcept
    {
        return (bool)rhs;
    }
}



