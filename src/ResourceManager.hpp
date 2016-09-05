#ifndef RESMANAGER_HPP
#define RESMANAGER_HPP

#include <map>
#include <queue>

#include "Texture.hpp"

namespace Beastmaster
{
	class ResourceManager
	{
	private:
		static ResourceManager instance;

		std::queue<Texture*> textureQueue;

		std::map<std::string, Texture*> textures;
		
	public:
		static ResourceManager& getInstance();

		void queueTexture(std::string name, std::string path);
		bool loadResources();

		Texture* getTexture(std::string name);		
	private:
		ResourceManager();
		~ResourceManager();
		ResourceManager(ResourceManager const&); //don't implement
		void operator=(ResourceManager const&); //don't implement	
	};
}

#endif