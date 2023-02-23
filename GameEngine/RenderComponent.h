#pragma once
#include <memory>
#include <string>
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	class RenderComponent : public BaseComponent
	{
	public:
		RenderComponent() = default;
		virtual ~RenderComponent() = default;

		void SetTexture(const std::string& filename);

		virtual void Initialize() override;

		virtual void Update() override;

		virtual void Render()const;

	protected:
		std::shared_ptr<Texture2D> m_texture{};
	};
}

