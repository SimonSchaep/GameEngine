#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "UIRenderComponent.h"
#include "SceneManager.h"
#include <iostream>

namespace engine
{

	GameObject::GameObject(const std::string& name, Scene* pScene)
		: m_Transform{ std::make_unique<Transform>(this) }
		, m_Name{ name }
		, m_pScene{ pScene }
	{
	}

	GameObject::~GameObject()
	{
		//if has parent
		if (GetParent())
		{
			//remove from parent
			SetParent(nullptr, true);
		}
		//if has children
		if (GetChildren().size() > 0)
		{
			//remove all children from parent
			for (int i{ int(GetChildren().size() - 1)}; i >= 0; --i) //reverse order cause removing items from array we are looping over
			{
				GetChildren()[i]->SetParent(nullptr, true);
			}
		}
	}

	void GameObject::Initialize()
	{
		//std::cout << GetName() << "init\n";
		for (auto& c : m_Components)
		{
			c->InitializeComponent();
		}
		m_IsInitialized = true;
	}

	void GameObject::Update()
	{
		if (!m_IsInitialized)
		{
			Initialize();
		}

		//Update Components
		for (size_t i{}; i < m_Components.size(); ++i)
		{
			m_Components[i]->UpdateComponent();
			if (m_Components[i]->IsMarkedForDeletion())
			{
				m_ToDeleteIndexes.push_back(i);
			}
		}

		//Delete Components - reverse order because it removes Components by index
		//If the Components at idx 0 and 1 need to be deleted and the one at 0 is deleted first, the Components at idx 1 will be at 0 when we delete it
		for (int i{ int(m_ToDeleteIndexes.size()) - 1 }; i >= 0; --i)
		{
			m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), m_Components[m_ToDeleteIndexes[i]]));
		}
		m_ToDeleteIndexes.clear();
	}

	void GameObject::RenderUI()
	{
		for (auto& renderer : m_UIRenderComponents)
		{
			renderer->RenderUI();
		}
	}

	Scene* GameObject::GetScene() const
	{
		if (m_IsSceneIndependant)
		{
			return SceneManager::GetInstance().GetActiveScene();
		}
		return m_pScene;
	}

	void GameObject::Render()const
	{
		for (auto& renderer : m_RenderComponents)
		{
			renderer->Render();
		}
	}

	void GameObject::MarkForDeletion(bool includeChildren)
	{
		m_IsMarkedForDeletion = true;
		if (includeChildren)
		{
			for (auto pChild : m_Children)
			{
				pChild->MarkForDeletion(includeChildren);
			}
		}
	}

	void GameObject::SetParent(GameObject* pParent, bool keepWorldPosition)
	{
		if (!pParent && keepWorldPosition)
		{
			m_Transform->SetLocalPosition(m_Transform->GetWorldPosition());
			//no need to set dirty, since if there is no parent, dirty will set local = to world, which is the case right now
		}
		else
		{
			if (keepWorldPosition)
			{
				m_Transform->SetLocalPosition(m_Transform->GetLocalPosition() - pParent->GetTransform()->GetWorldPosition());
			}
			m_Transform->SetDirty(true);
		}

		if (m_Parent)
		{
			m_Parent->RemoveChild(this);
		}
		m_Parent = pParent;
		if (m_Parent)
		{
			m_Parent->AddChild(this);
		}
	}

}