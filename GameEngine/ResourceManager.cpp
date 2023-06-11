#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"
#include "Sprite.h"
#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace engine
{

	void ResourceManager::Init(const std::string& dataPath)
	{
		m_DataPath = dataPath;

		if (TTF_Init() != 0)
		{
			throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
		}
	}

	std::unique_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file) const
	{
		const auto fullPath = m_DataPath + file;
		auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
		}
		return std::make_unique<Texture2D>(texture);
	}

	std::unique_ptr<Font> ResourceManager::LoadFont(const std::string& file, unsigned int size) const
	{
		return std::make_unique<Font>(m_DataPath + file, size);
	}

	std::shared_ptr<Sprite> ResourceManager::LoadSprite(const std::string& fileName, int cols, int rows, float timePerFrame, int startFrame, int endFrame, bool loop, bool reverse, bool flipX, bool flipY) const
	{
		return std::make_shared<Sprite>(fileName, cols, rows, timePerFrame, startFrame, endFrame, loop, reverse, flipX, flipY);
	}
}