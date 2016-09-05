#include "StateManager.hpp"

#include "GameApp.hpp"
#include "Log.hpp"

using namespace Beastmaster;

StateManager StateManager::instance;

StateManager& StateManager::getInstance()
{
	return StateManager::instance;
}

StateManager::StateManager()
{
	this->fading = false;
	this->fadeProgress = 0.0f;	
	this->fadeSpeed = 1.0f;
}
StateManager::~StateManager()
{
	//empty the state stack
	while (!this->states.empty())
	{
		GameState* topState = this->states.back();
		delete topState;

		this->states.pop_back();
	}
}

void StateManager::pushState(GameState* state, bool fade)
{
	if (this->fading)
		return;

	if (!fade)
	{
		this->pushState(state);
		return;
	}

	this->nextState = state;
	this->stateCommand = StateCommand::Push;

	this->StartFade(FadeStage::Out);
}
void StateManager::popState(bool fade)
{
	if (this->fading)
		return;

	if (!fade)
	{
		this->popState();
		return;
	}

	this->nextState = nullptr;
	this->stateCommand = StateCommand::Pop;

	this->StartFade(FadeStage::Out);
}
void StateManager::changeState(GameState* state, bool fade)
{
	if (this->fading)
		return;

	if (!fade)
	{
		this->changeState(state);
		return;
	}

	this->nextState = state;
	this->stateCommand = StateCommand::Change;

	this->StartFade(FadeStage::Out);
}

void StateManager::pushState(GameState* state)
{
	this->states.push_back(state);

	state->loadResources();
	state->update(0);
}
void StateManager::popState()
{
	GameState* topState = this->states.back();
	delete topState;

	this->states.pop_back();
}
void StateManager::changeState(GameState* state)
{
	//empty the state stack
	while (!this->states.empty())
	{
		GameState* topState = this->states.back();
		delete topState;

		this->states.pop_back();
	}

	this->states.push_back(state);

	state->loadResources();
	state->update(0);
}

void StateManager::handleEvent(SDL_Event& e)
{
	if (this->fading)
		return;

	//only handle events for the top state
	this->states.back()->handleEvent(e);
}

void StateManager::update(float deltaTime)
{
	if (this->fading)
	{
		this->fadeProgress += deltaTime * this->fadeSpeed;
		if (this->fadeProgress >= 1.0f)
		{
			if (this->fadeStage == FadeStage::Out)
			{
				this->StartFade(FadeStage::In);

				if (this->stateCommand == StateCommand::Push)
					this->pushState(this->nextState);
				else if (this->stateCommand == StateCommand::Pop)
					this->popState();
				else if (this->stateCommand == StateCommand::Change)
					this->changeState(this->nextState);

				this->nextState = nullptr;
			}
			else
			{
				this->fading = false;
			}
		}
		return;
	}

	//only update the top state
	this->states.back()->update(deltaTime);

}
void StateManager::render()
{
	//render all the states
	std::vector<GameState*>::iterator it = this->states.begin();
	for (it; it != this->states.end(); it++)
	{
		(*it)->render();
	}

	if (this->fading)
	{
		float fadeAmount = this->fadeProgress;
		if (this->fadeStage == FadeStage::In)
			fadeAmount = 1.0f - this->fadeProgress;

		GameApp::getInstance().renderRect(ivec2(0, 0), ivec2(1000, 1000), 0, 0, 0, fadeAmount * 255);
	}
}

void StateManager::StartFade(FadeStage fadeStage)
{
	this->fadeStage = fadeStage;
	this->fadeProgress = 0.0f;
	this->fading = true;
}