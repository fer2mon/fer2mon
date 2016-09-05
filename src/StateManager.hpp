#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <vector>

#include "GameState.hpp"

namespace Beastmaster
{
	enum FadeStage
	{
		In,
		Out
	};
	enum StateCommand
	{
		Push,
		Pop,
		Change
	};
	class StateManager
	{
	private:
		static StateManager instance;

		std::vector<GameState*> states;

		float fadeSpeed;

		bool fading;
		float fadeProgress;
		FadeStage fadeStage;
		StateCommand stateCommand;
		GameState* nextState;

	public:
		static StateManager& getInstance();

		void pushState(GameState* state, bool fade);
		void popState(bool fade);
		void changeState(GameState* state, bool fade);

		void handleEvent(SDL_Event& e);
		void update(float deltaTime);
		void render();

	private:
		StateManager();
		~StateManager();
		StateManager(StateManager const&); //don't implement
		void operator=(StateManager const&); //don't implement	

		void pushState(GameState* state);
		void popState();
		void changeState(GameState* state);

		void StartFade(FadeStage fadeStage);
	};
}

#endif