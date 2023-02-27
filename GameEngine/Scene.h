#pragma once
#include "SceneManager.h"
#include "GameObject.h"

class Scene final
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:
	void Add(std::unique_ptr<GameObject> object);
	void Remove(std::unique_ptr<GameObject> object);
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
	explicit Scene(const std::string& name);

	std::vector<size_t> m_ToDeleteIndexes{};

	std::string m_name;
	std::vector < std::unique_ptr<GameObject>> m_objects{};

	static unsigned int m_idCounter;
};
