#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <SDL.h>
#include <string>

namespace Beastmaster
{
	class Texture
	{
	private:
		std::string name;
		std::string path;

		SDL_Texture* data;

		int width, height;
	public:
		Texture(std::string name, std::string path);
		~Texture();

		bool loadData();

		inline SDL_Texture* getData() { return this->data; }
		inline std::string getName() { return this->name; }
		inline std::string getPath() { return this->path; }

		inline int getWidth() { return this->width; }
		inline int getHeight() { return this->height; }
	};
}

#endif