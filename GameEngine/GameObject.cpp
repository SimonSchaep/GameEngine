#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "RenderComponent.h"
#include "UIRenderComponent.h"
#include "SceneManager.h"
#include <iostream>
#include "ServiceLocator.h"
#include "Logger.h"

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

	void GameObject::AddChild(GameObject* pGameObject)
	{
		pGameObject->SetHierarchyIsSceneIndependant(m_IsSceneIndependant);
		m_Children.emplace_back(pGameObject);
	}

	void GameObject::RemoveChild(GameObject* pGameObject)
	{
		m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), pGameObject));
	}

	void GameObject::SetHierarchyIsSceneIndependant(bool isDependant)
	{
		m_IsSceneIndependant = isDependant;
		for (auto child : m_Children)
		{
			child->SetHierarchyIsSceneIndependant(isDependant);
		}
	}

	void GameObject::Initialize()
	{
		//no range-based for, cause than iterator would get invalidated if components get added during initailize
		for (size_t i{}; i < m_Components.size(); ++i)
		{
			m_Components[i]->InitializeComponent();
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
			if (!m_Components[i]->IsActive())continue;

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
			if (!renderer->IsActive())continue;

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
			if (!renderer->IsActive())continue;

			renderer->Render();
		}
	}

	void GameObject::MarkAsSceneIndependant()
	{
		if (m_pParent)
		{
			ServiceLocator::GetLogger().LogLine("marking a child as scene independant doesn't work, try marking the parent gameobject");
		}
		else
		{
			SetHierarchyIsSceneIndependant(true);
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

		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}
		m_pParent = pParent;
		if (m_pParent)
		{
			m_pParent->AddChild(this);
		}
	}

}