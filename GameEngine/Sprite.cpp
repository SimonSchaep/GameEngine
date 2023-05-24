#include "Sprite.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include <iostream>

engine::Sprite::Sprite(const std::string& fileName, int cols, int rows, float timePerFrame, int startFrame, int endFrame, bool loop, bool reverse, bool flipX, bool flipY)
	:m_Texture{ResourceManager::GetInstance().LoadTexture(fileName)}
	, m_Cols{cols}
	, m_Rows{rows}
	, m_TimePerFrame{timePerFrame}
	, m_StartFrame{startFrame}
	, m_EndFrame{endFrame}
	, m_Loop{loop}
	, m_Reverse{ reverse }
	, m_FlipX{flipX}
	, m_FlipY{flipY}
{
}

void engine::Sprite::Update(float speed)
{
	m_Timer += TimeManager::GetInstance().GetDeltaTime() * speed;

	if (m_Timer > m_TimePerFrame)
	{
		// Go to next frame
		if (m_Reverse)
		{
			--m_CurrentFrame;
			// end frame and start frame switched cause reverse
			if (m_CurrentFrame < m_StartFrame)
			{
				if (!m_Loop)
				{
					++m_CurrentFrame;
					m_IsPaused = true;
				}
				else
				{
					m_CurrentFrame = m_EndFrame;
				}
			}
		}
		else
		{
			++m_CurrentFrame;
			if (m_CurrentFrame > m_EndFrame)
			{
				if (!m_Loop)
				{
					--m_CurrentFrame;
					m_IsPaused = true;
				}
				else
				{
					m_CurrentFrame = m_StartFrame;
				}
			}
		}

		// Only keep the remaining time
		m_Timer -= m_TimePerFrame;
	}
}

void engine::Sprite::Render(const structs::Rect& dstRect) const
{
	if (!m_Texture)
	{
		return;
	}

	int textureWidth;
	int textureHeight;
	SDL_QueryTexture(m_Texture->GetSDLTexture(), nullptr, nullptr, &textureWidth, &textureHeight);
	float frameWidth{ float(textureWidth) / m_Cols };
	float frameHeight{ float(textureHeight) / m_Rows };

	structs::Rect srcRect;
	srcRect.Height = frameHeight;
	srcRect.Width = frameWidth;
	srcRect.BottomLeft.x = m_CurrentFrame % m_Cols * srcRect.Width;
	srcRect.BottomLeft.y = m_CurrentFrame / m_Cols * srcRect.Height + srcRect.Height;

	if (dstRect.Width != 0 && dstRect.Height != 0)
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, dstRect, srcRect, m_FlipX, m_FlipY);
	}
	else
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, dstRect.BottomLeft.x, dstRect.BottomLeft.y, srcRect, m_FlipX, m_FlipY);
	}
}

void engine::Sprite::Reset()
{
	m_CurrentFrame = m_StartFrame;
	m_Timer = 0;
}
