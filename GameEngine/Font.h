#pragma once
#include <string>

namespace engine
{
	/**
		 * Simple RAII wrapper for a _TTF_Font
		 */
	class Font final
	{
	public:
		TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, unsigned int size);
		~Font();

		Font(const Font&) = delete;
		Font(Font&&) = delete;
		Font& operator= (const Font&) = delete;
		Font& operator= (const Font&&) = delete;
	private:
		TTF_Font* m_Font;
		unsigned int m_Size;
	};

}