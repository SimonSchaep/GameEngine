#pragma once
#include <glm/glm.hpp>

class GameObject;

class Transform final
{
public:
	Transform(GameObject* pGameObject);

	void SetDirty(bool dirty);

	const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
	const glm::vec3& GetWorldPosition();
	void SetLocalPosition(float x, float y, float z);
	void SetLocalPosition(const glm::vec3& pos);
	
private:
	GameObject* m_pGameObject{};

	glm::vec3 m_LocalPosition;
	glm::vec3 m_Worldposition;

	bool m_IsDirty;
};