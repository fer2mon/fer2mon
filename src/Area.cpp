#include "Area.hpp"

#include "GameApp.hpp"
#include "Log.hpp"
#include "ResourceManager.hpp"
#include "tinyxml2.h"
#include "Utility.hpp"

using namespace Beastmaster;

Area::Area(std::string path)
{
	this->path = path;
}
Area::~Area()
{
	
}
bool Area::loadData()
{
	tinyxml2::XMLDocument doc; 

	if (doc.LoadFile(this->path.c_str()) != 0) //todo: load area from stream instead using builtin functionality
	{
		Log::getInstance().log("Unable to load area " + this->path, LogEntryType::Error);
		return false;
	}

	tinyxml2::XMLElement* mapElement = doc.FirstChildElement("map");
	if (!mapElement)
	{
		Log::getInstance().log("No map element present in area " + this->path, LogEntryType::Error);
		return false;
	}

	this->width = mapElement->IntAttribute("width");
	this->height = mapElement->IntAttribute("height");

	//parse tilesets
	for (tinyxml2::XMLElement* tilesetElement = mapElement->FirstChildElement("tileset"); tilesetElement != 0; tilesetElement = tilesetElement->NextSiblingElement("tileset"))
	{
		const char* name = tilesetElement->Attribute("name");
		if (name == NULL)
		{
			Log::getInstance().log("No 'name' attribute in tileset definition in area " + this->path, LogEntryType::Error);
			return false;
		}

		Texture* tilesetTexture = ResourceManager::getInstance().getTexture(std::string(name));
		if (tilesetTexture == nullptr)
		{
			Log::getInstance().log("Area is trying to use tileset which is not loaded: " + this->path, LogEntryType::Error);
			return false;
		}
		this->tileset = tilesetTexture;
	}		

	for (tinyxml2::XMLElement* layerElement = mapElement->FirstChildElement("layer"); layerElement != 0; layerElement = layerElement->NextSiblingElement("layer"))
	{		
		std::string text = std::string(layerElement->FirstChildElement()->GetText());

		//remove newlines
		text.erase(std::remove(text.begin(), text.end(), '\n'), text.end());

		//split to tokens and convert to ints so they can be converted to array
		std::vector<std::string> tokens = Utility::split(text, ',');
		std::vector<int> layerData;
		for (int i = 0; i < tokens.size(); i++)
		{			
			layerData.push_back(std::stoi(tokens[i]));
		}

		this->layers.push_back(layerData);
	}

	tinyxml2::XMLElement* objectGroupElement = mapElement->FirstChildElement("objectgroup");

	for (tinyxml2::XMLElement* objectElement = objectGroupElement->FirstChildElement("object"); objectElement != 0; objectElement = objectElement->NextSiblingElement("object"))
	{
		AreaObject object;
		object.name = objectElement->Attribute("name");
		object.position = ivec2();
		object.position.x = objectElement->IntAttribute("x") / TILE_SIZE;
		object.position.y = objectElement->IntAttribute("y") / TILE_SIZE - 1; //due to Tiled's nature, we need to substract 1 tile vertically

		Log::getInstance().log("Loaded object: " + object.name + " at (" + std::to_string(object.position.x) + ", " + std::to_string(object.position.y) + ").", LogEntryType::Normal);

		//properties
		tinyxml2::XMLElement* propertiesElement = objectElement->FirstChildElement("properties");

		if (propertiesElement != 0)
		{
			for (tinyxml2::XMLElement* propertyElement = propertiesElement->FirstChildElement("property"); propertyElement != 0; propertyElement = propertyElement->NextSiblingElement("property"))
			{
				object.properties[propertyElement->Attribute("name")] = propertyElement->Attribute("value");
			}
		}

		this->objects.push_back(object);
	}

	return true;
}

void Area::renderLayer(int layer, ivec2 camera)
{	
	//simple culling based on camera and screen dimensions
	ivec2 cameraTileSpace = ivec2(camera.x / TILE_SIZE, camera.y / TILE_SIZE);
	ivec2 screenScaleTileSpace = GameApp::getInstance().getResolution() / TILE_SIZE;
	screenScaleTileSpace += cameraTileSpace + ivec2(1,1);

	for (int y = cameraTileSpace.y; y < screenScaleTileSpace.y; y++)
	{
		for (int x = cameraTileSpace.x; x < screenScaleTileSpace.x; x++)
		{
			ivec2 position = ivec2(x * TILE_SIZE, y * TILE_SIZE);
			ivec2 scale(TILE_SIZE, TILE_SIZE);

			int tileIndex = this->getTile(layer, ivec2(x, y));

			if (tileIndex == TILE_EMPTY)
				continue;

			if (tileIndex == -1) //empty tile (transparent)
				continue;

			ivec2 count(this->tileset->getWidth() / TILE_SIZE, this->tileset->getHeight() / TILE_SIZE);

			ivec2 offset;
			offset.x = (tileIndex % count.x) * TILE_SIZE;
			offset.y = (tileIndex / count.x) * TILE_SIZE;			

			GameApp::getInstance().renderTexture(this->tileset, position - camera, offset, scale);
		}
	}
}

int Area::getTile(int layer, ivec2 position)
{
	if (position.x < 0 || position.x >= this->width || position.y < 0 || position.y >= this->height)
	{
		if (layer == LAYER_COLLISION)
			return !TILE_WALKABLE;
		else
			return TILE_EMPTY;
	}

	if (layer == LAYER_COLLISION) //we don't substract values for this layer (0 is walkable, everything else is not)
		return this->layers[layer][position.x + position.y*this->width];

	//substract 1 because tiled arrays begin with 1, not 0
	return this->layers[layer][position.x + position.y*this->width] - 1;
}
ivec2 Area::getSize()
{
	return ivec2(this->width, this->height);
}
Texture* Area::getTilesetTexture()
{
	return this->tileset;
}

AreaObject* Area::findObject(std::string name)
{
	for (int i = 0; i < this->objects.size(); i++)
	{
		if (this->objects[i].name == name)
			return &this->objects[i];
	}
	return nullptr;
}

AreaObject* Area::getObject(ivec2 position)
{
	for (int i = 0; i < this->objects.size(); i++)
	{
		if (this->objects[i].position == position)
			return &this->objects[i];
	}
	return nullptr;
}