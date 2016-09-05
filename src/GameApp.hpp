#ifndef GAMEAPP_HPP
#define GAMEAPP_HPP

#include <SDL.h>
#include <SDL_image.h>

#include "Vec2.hpp"

#define PATH_MAPS "data/maps/"

namespace Beastmaster
{
	class Texture;

	struct Clock
	{		
		Uint32 current = 0;
		Uint32 last = 0;
		float deltaTime = 0;

		inline void tick()
		{
			last = current;
			current = SDL_GetTicks();

			deltaTime = (current - last) * 0.001f;
		}
	};

	class GameApp
	{
	private:
		static GameApp instance;

		SDL_Window* window;
		SDL_Renderer* renderer;

		bool running;

		int scale;

	public:
		int start(int scale);
		void handleEvent(SDL_Event& e);
		void update(float deltaTime);
		void render();

		void renderTexture(Texture* texture, ivec2 position, ivec2 offset, ivec2 scale);
		void renderRect(ivec2 position, ivec2 scale, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

		static GameApp& getInstance();

		SDL_Renderer* getRenderer();
		ivec2 getResolution();
		int getScale();

	private:
		GameApp();
		~GameApp();
		GameApp(GameApp const&); //don't implement
		void operator=(GameApp const&); //don't implement		
	};
}

#endif