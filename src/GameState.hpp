#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SDL.h>

namespace Beastmaster
{
	class GameState
	{
	public:
		virtual void loadResources() = 0;
		virtual void handleEvent(SDL_Event& e) = 0;
		virtual void update(float deltaTime) = 0;
		virtual void render() = 0;		
	};
}

#endif