#pragma once
#include "UIRenderComponent.h"
#include <vector>

class TrashTheCacheComponent : public engine::UIRenderComponent
{
	struct Transform
	{
		float matrix[16] = {
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1
		};
	};

	class GameObject3D
	{
	public:
		Transform Transform{};
		int ID{};
	};

	class GameObject3DAlt
	{
	public:
		Transform* pTransform{};
		int ID{};
	};

public:
	TrashTheCacheComponent(engine::GameObject* pGameObject) :UIRenderComponent(pGameObject) {};
	virtual ~TrashTheCacheComponent() = default;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void RenderUI() override;

private:
	void MeasureExercise1(int count);
	void MeasureExercise2(int count);
	void MeasureExercise2Alt(int count);

	std::vector<int> m_Ex1Measurements{};
	std::vector<int> m_Ex2Measurements{};
	std::vector<int> m_Ex2AltMeasurements{};

	

	std::vector<const char*> m_Labels{
		"1",
		"2",
		"4",
		"8",
		"16",
		"32",
		"64",
		"128",
		"256",
		"512",
		"1024"
	};
	std::vector<double> m_LabelValues{
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8,
		9,
		10
	};

};

