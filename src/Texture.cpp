#include "Texture.hpp"

#include <SDL_image.h>

#include "GameApp.hpp"
#include "Log.hpp"

using namespace Beastmaster;

Texture::Texture(std::string name, std::string path)
{
	this->name = name;
	this->path = path;
	this->data = nullptr;
}
Texture::~Texture()
{
	if (this->data != nullptr)
		SDL_DestroyTexture(this->data);
}
bool Texture::loadData()
{
	this->data = IMG_LoadTexture(GameApp::getInstance().getRenderer(), this->path.c_str());
	if (this->data == nullptr)
		return false;

	SDL_QueryTexture(this->data, nullptr, nullptr, &this->width, &this->height);

	return true;
}