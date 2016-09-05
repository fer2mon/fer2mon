#include "StateBattle.hpp"

#include "ResourceManager.hpp"
#include "StateManager.hpp"
#include "StateOverworld.hpp"

using namespace Beastmaster;

StateBattle::StateBattle()
{
	this->font = new BitmapFont(ResourceManager::getInstance().getTexture("font_main_black"), 128, 256, 18, 7, 32);
}
StateBattle::~StateBattle()
{
	delete font;
}
void StateBattle::loadResources()
{
}
void StateBattle::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		StateManager::getInstance().popState(true);
	}
}
void StateBattle::update(float deltaTime)
{

}
void StateBattle::render()
{
	font->RenderCharacter('B', ivec2(8, 0));
}