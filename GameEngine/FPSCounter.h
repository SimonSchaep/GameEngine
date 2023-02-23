#pragma once
#include "BaseComponent.h"
#include <memory>

namespace dae
{
	class TextRenderComponent;

	class FPSCounter : public BaseComponent
	{
	public:
		FPSCounter() = default;
		virtual ~FPSCounter() = default;

		virtual void Initialize() override;
		virtual void Update() override;

	private:
		std::shared_ptr<TextRenderComponent> m_TextRenderComponent{};
	};
}