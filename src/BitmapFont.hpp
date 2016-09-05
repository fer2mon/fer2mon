#ifndef BITMAPFONT_HPP
#define BITMAPFONT_HPP

#include "Texture.hpp"
#include "Vec2.hpp"

namespace Beastmaster
{
	class BitmapFont
	{
	private:
		Texture* texture;

		int width, height, countX, countY;
		int characterWidth, characterHeight;
		int asciiOffset;
	public:
		BitmapFont(Texture* texture, int width, int height, int countX, int countY, int asciiOffset);
		void RenderCharacter(char c, ivec2 position);
		void RenderString(std::string text, ivec2 position);

		ivec2 getDimensions();
	};
}

#endif