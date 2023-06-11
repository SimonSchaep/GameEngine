#pragma once
#include <glm/glm.hpp>

namespace engine
{
	//todo: implement scaling
	//todo: implement rotation

	class GameObject;

	class Transform final
	{
	public:
		Transform(GameObject* pGameObject);

		void SetDirty(bool dirty);

		const glm::vec2& GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec2& GetWorldPosition();
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec2& pos);
		void SetWorldPosition(float x, float y);
		void SetWorldPosition(const glm::vec2& pos);
		void Translate(const glm::vec2& offset);
		void Translate(float x, float y);

	private:
		GameObject* m_pGameObject{};

		glm::vec2 m_LocalPosition{};
		glm::vec2 m_Worldposition{};

		bool m_IsDirty{};
	};

}