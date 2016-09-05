#ifndef STATEBATTLE_HPP
#define STATEBATTLE_HPP

#include "BitmapFont.hpp"
#include "GameState.hpp"

namespace Beastmaster
{
	class StateBattle : public GameState
	{
	private:
		BitmapFont* font;
	public:
		StateBattle();
		~StateBattle();
		virtual void loadResources();
		virtual void handleEvent(SDL_Event& e);
		virtual void update(float deltaTime);
		virtual void render();
	};
}

#endif