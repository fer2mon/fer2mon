#ifndef STATEOVERWORLD_HPP
#define STATEOVERWORLD_HPP

#include "Area.hpp"
#include "BitmapFont.hpp"
#include "Character.hpp"
#include "GameState.hpp"

namespace Beastmaster
{
	class StateOverworld : public GameState
	{
	private:
		Area* area;
		int portalIndex;

		BitmapFont* font;

		Character* playerCharacter;

		ivec2 camera;
	public:
		StateOverworld(std::string areaFilename, int portalIndex);
		~StateOverworld();
		virtual void loadResources();
		virtual void handleEvent(SDL_Event& e);
		virtual void update(float deltaTime);
		virtual void render();

	private:
		void activateObjectAt(ivec2 position);
	};
}

#endif