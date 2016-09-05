#ifndef STATEMESSAGE_HPP
#define STATEMESSAGE_HPP

#include <vector>

#include "BitmapFont.hpp"
#include "GameState.hpp"

namespace Beastmaster
{
	class StateMessage : public GameState
	{
	private:
		BitmapFont* font;

		Texture* box;
		ivec2 boxPosition;
		ivec2 boxScale;

		float typeSpeed;
		float typeTimer;
		int typeIndex;
		int typeLine;
		bool typingOver;

		std::vector<std::string> lines;
	public:
		StateMessage(std::string message);
		~StateMessage();
		virtual void loadResources();
		virtual void handleEvent(SDL_Event& e);
		virtual void update(float deltaTime);
		virtual void render();
	};
}

#endif