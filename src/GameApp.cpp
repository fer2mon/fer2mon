#include "GameApp.hpp"

#include <iostream>

#include "Log.hpp"
#include "ResourceManager.hpp"
#include "StateManager.hpp"
#include "StateOverworld.hpp" //remove later, only for testing

#define RESX 240
#define RESY 160

using namespace Beastmaster;

GameApp GameApp::instance;

GameApp& GameApp::getInstance()
{
	return GameApp::instance;
}

GameApp::GameApp()
{
	this->running = true;
}

GameApp::~GameApp()
{
	IMG_Quit();
	SDL_Quit();

	if (this->window != 0)
		SDL_DestroyWindow(this->window);

	if (this->renderer != 0)
		SDL_DestroyRenderer(this->renderer);
}

int GameApp::start(int scale)
{
	this->scale = scale;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Log::getInstance().log("Unable to initialize SDL.", LogEntryType::Error);
		return 1;
	}

	this->window = SDL_CreateWindow("Beastmaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RESX*scale, RESY*scale, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (this->window == 0)
	{
		Log::getInstance().log("Unable to initialize windowing system.", LogEntryType::Error);
		return 2;
	}

	this->renderer = SDL_CreateRenderer(this->window, -1, 0);
	if (this->renderer == 0)
	{
		Log::getInstance().log("Unable to create renderer.", LogEntryType::Error);
		return 3;
	}

	SDL_Texture* renderTarget = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RESX, RESY);
	
	
	//renderer settings
	SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetScale(this->renderer, scale, scale);

	ResourceManager::getInstance().queueTexture("player", "data/player.png");
	ResourceManager::getInstance().queueTexture("font_main_black", "data/font5x7_black.png");
	ResourceManager::getInstance().queueTexture("font_main_white", "data/font5x7_white.png");
	ResourceManager::getInstance().queueTexture("conversation_box", "data/conversation_box.png");
	ResourceManager::getInstance().queueTexture("tileset_01", "data/tileset_01.png");
	if (!ResourceManager::getInstance().loadResources())
	{
		Log::getInstance().log("Resource loading failed.", LogEntryType::Error);
		return 4;
	}

	StateManager::getInstance().pushState(new StateOverworld("map_test.tmx", 0), false);

	Clock clock;

	SDL_Event e;
	while (this->running)
	{
		clock.tick();

		while (SDL_PollEvent(&e))
		{
			this->handleEvent(e);
		}

		this->update(clock.deltaTime);
		//this->update(1 / 600.0f);

		SDL_SetRenderTarget(this->renderer, renderTarget);
		SDL_RenderClear(this->renderer);	
		this->render();

		SDL_SetRenderTarget(this->renderer, NULL);
		SDL_RenderClear(this->renderer);
		SDL_RenderCopy(this->renderer, renderTarget, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_RenderPresent(this->renderer);

		
	}

	return 0;
}

void GameApp::handleEvent(SDL_Event& e)
{
	StateManager::getInstance().handleEvent(e);
}

void GameApp::update(float deltaTime)
{
	StateManager::getInstance().update(deltaTime);
}

void GameApp::render()
{
	StateManager::getInstance().render();
}

void GameApp::renderTexture(Texture* texture, ivec2 position, ivec2 offset, ivec2 scale)
{
	SDL_Texture* data = texture->getData();

	SDL_Rect src;
	src.x = offset.x;
	src.y = offset.y;
	src.w = scale.x;
	src.h = scale.y;

	SDL_Rect dst;
	dst.x = position.x;
	dst.y = position.y;
	dst.w = scale.x;
	dst.h = scale.y;

	SDL_RenderCopy(this->renderer, data, &src, &dst);
}

void GameApp::renderRect(ivec2 position, ivec2 scale, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_Rect rect;
	rect.x = position.x;
	rect.y = position.y;
	rect.w = scale.x;
	rect.h = scale.y;

	
	SDL_SetRenderDrawColor(this->renderer, r, g, b, a);

	SDL_RenderFillRect(this->renderer, &rect);

	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
}

SDL_Renderer* GameApp::getRenderer()
{
	return this->renderer;
}

ivec2 GameApp::getResolution()
{
	return ivec2(RESX, RESY);
}