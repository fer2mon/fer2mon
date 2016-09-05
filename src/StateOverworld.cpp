#include "StateOverworld.hpp"

#include "GameApp.hpp"
#include "Log.hpp"
#include "ResourceManager.hpp"
#include "StateBattle.hpp"
#include "StateManager.hpp"
#include "StateMessage.hpp"
#include "Utility.hpp"

using namespace Beastmaster;

StateOverworld::StateOverworld(std::string areaFilename, int portalIndex)
{
	areaFilename = PATH_MAPS + areaFilename;
	this->area = new Area(areaFilename);
	this->portalIndex = portalIndex;
	
	this->font = new BitmapFont(ResourceManager::getInstance().getTexture("font_main_black"), 128, 64, 18, 7, 32);	
}
StateOverworld::~StateOverworld()
{
	delete this->playerCharacter;
	delete this->area;
	delete this->font;
}
void StateOverworld::loadResources()
{
	if (!this->area->loadData())
	{
		Log::getInstance().log("Unable to load map.", LogEntryType::Error); //add error handling later
	}

	this->playerCharacter = new Character("Player", "player", this->area);

	AreaObject* playerStart = this->area->findObject("PORTAL " + std::to_string(portalIndex));
	if (playerStart == nullptr)
	{
		Log::getInstance().log("Map doesn't contain portal with index " + std::to_string(portalIndex) + ". Defaulting to (0, 0).", LogEntryType::Warning);
	}
	else
	{
		this->playerCharacter->setPosition(playerStart->position);
	}
}
void StateOverworld::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
		{
			this->activateObjectAt(this->playerCharacter->getPosition() + this->playerCharacter->getDirection());
		}
	}
}
void StateOverworld::update(float deltaTime)
{
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_LEFT])
		this->playerCharacter->move(ivec2(-1, 0));
	if (keystate[SDL_SCANCODE_RIGHT])
		this->playerCharacter->move(ivec2(1, 0));
	if (keystate[SDL_SCANCODE_UP])
		this->playerCharacter->move(ivec2(0, -1));
	if (keystate[SDL_SCANCODE_DOWN])
		this->playerCharacter->move(ivec2(0, 1));

	this->playerCharacter->update(deltaTime);

	this->camera = (ivec2)this->playerCharacter->getRenderPosition() - GameApp::getInstance().getResolution() / 2 + ivec2(16, 32) / 2; //half of character dimensions

	//check if player will be standing on something (trigger, portal...)
	if (this->playerCharacter->isMoving())
	{
		ivec2 nextPlayerPosition = this->playerCharacter->getNextPosition();

		this->activateObjectAt(nextPlayerPosition);
	}
}
void StateOverworld::render()
{
	this->area->renderLayer(LAYER_BACKGROUND_1, this->camera);
	this->area->renderLayer(LAYER_BACKGROUND_2, this->camera);

	this->font->RenderCharacter('A', ivec2(0, 0));

	this->playerCharacter->render(this->camera);

	this->area->renderLayer(LAYER_FOREGROUND_1, this->camera);
	this->area->renderLayer(LAYER_FOREGROUND_2, this->camera);
}

void StateOverworld::activateObjectAt(ivec2 position)
{
	AreaObject* object = this->area->getObject(position);
	if (object != nullptr)
	{
		std::vector<std::string> tokens = Utility::split(object->name, ' ');
		if (tokens[0] == "PORTAL")
		{
			//send the player to another map
			if (object->properties.size() > 0) //a portal with no properties is a player start location
			{
				std::string areaFilename = object->properties["Area Name"];
				int portalIndex = std::stoi(object->properties["Portal Index"]);

				StateManager::getInstance().changeState(new StateOverworld(areaFilename, portalIndex), true);
			}
		}
		else if (tokens[0] == "GOSSIP")
		{
			std::string gossip = object->properties["Gossip"];

			StateManager::getInstance().pushState(new StateMessage(gossip), false);
		}
	}
}