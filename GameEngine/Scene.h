#pragma once
#include "SceneManager.h"
#include "GameObject.h"

class Scene final
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:
	GameObject* CreateAndAddGameObject();
	void RemoveAll();

	void Initialize();
	void Update();
	void Render() const;

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private:
	void RemoveGameObjectByIndex(size_t i);

	explicit Scene(const std::string& name);

	std::vector<size_t> m_ToDeleteIndexes{};

	std::string m_Name;
	std::vector < std::unique_ptr<GameObject>> m_GameObjects{};

	static unsigned int m_IdCounter;
};
