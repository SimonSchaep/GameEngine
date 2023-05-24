#pragma once
#include <string>
#include <memory>
#include "Singleton.h"

namespace engine
{

	class Texture2D;
	class Font;
	class Sprite;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::unique_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::unique_ptr<Font> LoadFont(const std::string& file, unsigned int size) const;
		std::shared_ptr<Sprite> LoadSprite(const std::string& fileName, int cols, int rows, float timePerFrame, int startFrame, int endFrame, bool loop = false, bool reverse = false, bool flipX = false, bool flipY = false) const;
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_DataPath;
	};

}