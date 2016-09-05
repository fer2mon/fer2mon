#include "BitmapFont.hpp"

#include "GameApp.hpp"

using namespace Beastmaster;

BitmapFont::BitmapFont(Texture* texture, int width, int height, int countX, int countY, int asciiOffset)
{
	this->texture = texture;
	this->width = width;
	this->height = height;
	this->countX = countX;
	this->countY = countY;
	this->characterWidth = this->width / this->countX;
	this->characterHeight = this->height / this->countY;
	this->asciiOffset = asciiOffset;
}

void BitmapFont::RenderCharacter(char c, ivec2 position)
{
	ivec2 offset;

	c -= asciiOffset;

	offset.x = (c % this->countX) * this->characterWidth;
	offset.y = (c / this->countX) * this->characterHeight;

	ivec2 scale(this->characterWidth, this->characterHeight);

	GameApp::getInstance().renderTexture(this->texture, position, offset, scale);
}
void BitmapFont::RenderString(std::string text, ivec2 position)
{
	for (int i = 0; i < text.length(); i++)
	{
		this->RenderCharacter(text[i], position + ivec2(i*this->characterWidth, 0));
	}
}

ivec2 BitmapFont::getDimensions()
{
	return ivec2(this->characterWidth, this->characterHeight);
}