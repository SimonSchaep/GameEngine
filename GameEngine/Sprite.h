#pragma once
#include <memory>
#include <string>
#include "Structs.h"

namespace engine
{
	class Texture2D;

	class Sprite final
	{
	public:
		Sprite(const std::string& fileName, int cols, int rows, float timePerFrame, int startFrame, int endFrame, bool loop, bool reverse, bool flipX, bool flipY);
		~Sprite() = default;

		Sprite(const Sprite& other) = delete;
		Sprite(Sprite&& other) = delete;
		Sprite& operator=(const Sprite& other) = delete;
		Sprite& operator=(Sprite&& other) = delete;

		void Update(float speed);
		void Render(const structs::Rect& dstRect)const;

		void SetIsPaused(bool isPaused) { m_IsPaused = isPaused; }

		void SetFlipX(bool flipX) { m_FlipX = flipX; }
		void SetFlipY(bool flipY) { m_FlipY = flipY; }

		void Reset();

		Texture2D* GetTexture()const { return m_Texture.get(); }


	private:
		std::unique_ptr<Texture2D> m_Texture{};

		int m_Cols{};
		int m_Rows{};

		float m_TimePerFrame{};

		float m_Timer{};
		bool m_IsPaused{};

		int m_StartFrame{};
		int m_EndFrame{};

		int m_CurrentFrame{};

		bool m_Loop{};
		bool m_Reverse{};

		bool m_FlipX{};
		bool m_FlipY{};
	};
}