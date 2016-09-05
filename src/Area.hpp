#ifndef AREA_HPP
#define AREA_HPP

#include <map>
#include <vector>

#include "Texture.hpp"
#include "Vec2.hpp"

#define LAYER_BACKGROUND_1 0
#define LAYER_BACKGROUND_2 1
#define LAYER_FOREGROUND_1 2
#define LAYER_FOREGROUND_2 3
#define LAYER_COLLISION 4

#define TILE_EMPTY 0
#define TILE_WALKABLE 0

#define TILE_SIZE 16

namespace Beastmaster
{	
	struct AreaObject
	{
		std::string name;
		ivec2 position;
		std::map<std::string, std::string> properties;
	};
	class Area
	{
	private:
		std::string path;

		int width, height;
		Texture* tileset;

		std::vector<std::vector<int>> layers;
		std::vector<AreaObject> objects;
	public:
		Area(std::string path);		
		~Area();

		bool loadData();

		void renderLayer(int layer, ivec2 camera);

		int getTile(int layer, ivec2 position);
		ivec2 getSize();
		Texture* getTilesetTexture();

		AreaObject* findObject(std::string name);
		AreaObject* getObject(ivec2 position);
	};
}

#endif