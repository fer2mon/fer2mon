#include "ResourceManager.hpp"

#include "Log.hpp"

using namespace Beastmaster;

ResourceManager ResourceManager::instance;

ResourceManager& ResourceManager::getInstance()
{
	return ResourceManager::instance;
}

ResourceManager::ResourceManager()
{
}
ResourceManager::~ResourceManager()
{
	//	in case there's some textures left in queue
	while (!this->textureQueue.empty())
	{
		Texture* front = this->textureQueue.front();
		delete front;
		this->textureQueue.pop();
	}
}

void ResourceManager::queueTexture(std::string name, std::string path)
{
	this->textureQueue.push(new Texture(name, path));
}
bool ResourceManager::loadResources()
{
	while (!this->textureQueue.empty())
	{
		Texture* front = this->textureQueue.front();

		//check if the texture with the same name isn't already loaded
		if (getTexture(front->getName()) == nullptr)
		{
			Log::getInstance().log("Loading texture '" + front->getPath() + "' as '" + front->getName() + "'.", LogEntryType::Normal);
			if (!front->loadData())
			{
				Log::getInstance().log("Unable to load texture '" + front->getPath() + "'.", LogEntryType::Error);
				return false;
			}
			textures[front->getName()] = front;
		}
		else
		{
			Log::getInstance().log("Texture with name '" + front->getName() + "' already loaded. Skipping.", LogEntryType::Warning);
		}

		this->textureQueue.pop();
	}
	return true;
}

Texture* ResourceManager::getTexture(std::string name)
{
	std::map<std::string, Texture*>::iterator it = this->textures.find(name);
	if (it != this->textures.end())
	{
		return it->second;
	}
	return nullptr;
}