#include "StateMessage.hpp"

#include "GameApp.hpp"
#include "ResourceManager.hpp"
#include "StateManager.hpp"
#include "Utility.hpp"

using namespace Beastmaster;

#define LINE_LENGTH 32

StateMessage::StateMessage(std::string message)
{
	this->font = new BitmapFont(ResourceManager::getInstance().getTexture("font_main_black"), 128, 64, 18, 7, 32);
	this->box = ResourceManager::getInstance().getTexture("conversation_box");
	this->boxPosition = ivec2(0, GameApp::getInstance().getResolution().y - this->box->getHeight());
	this->boxScale = ivec2(this->box->getWidth(), this->box->getHeight());

	std::vector<std::string> tokens = Utility::split(message, ' ');

	for (int i = 0; i < tokens.size(); i++)
	{
		if (lines.size() == 0)
			lines.push_back("");

		if (lines.back().length() == 0)
		{
			lines.back() += tokens[i] + " ";
			continue;
		}
		else
		{
			std::string temp = lines.back() + tokens[i];
			if (temp.length() <= LINE_LENGTH)
			{
				lines.back() = temp + " ";
			}
			else
			{
				lines.push_back(tokens[i] + " ");
			}
		}
	}

	this->typeLine = 0;
	this->typeIndex = 0;
	this->typeTimer = 0.0f;
	this->typeSpeed = 30.0f;
	this->typingOver = false;
}
StateMessage::~StateMessage()
{
	delete font;
}
void StateMessage::loadResources()
{
}
void StateMessage::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN)
	{
		if (this->typingOver == false)
			this->typingOver = true;
		else
			StateManager::getInstance().popState(false);
	}
}
void StateMessage::update(float deltaTime)
{
	if (!typingOver)
	{
		this->typeTimer += deltaTime * this->typeSpeed;
		if (this->typeTimer >= 1.0f)
		{
			this->typeTimer = 0.0f;
			this->typeIndex++;
			if (this->typeIndex >= this->lines[this->typeLine].length())
			{
				this->typeIndex = 0;
				this->typeLine++;
				if (this->typeLine == this->lines.size())
				{
					this->typingOver = true;;
				}
			}
		}
	}
}
void StateMessage::render()
{
	GameApp::getInstance().renderTexture(this->box, this->boxPosition, ivec2(0, 0), this->boxScale);

	for (int i = 0; i < this->lines.size(); i++)
	{
		std::string text = "";
		if (!typingOver)
		{			
			if (i == this->typeLine)
				text = this->lines[i].substr(0, this->typeIndex);
			else if (i < this->typeLine)
				text = this->lines[i];
		}
		else
			text = this->lines[i];

		font->RenderString(text, this->boxPosition + ivec2(8, 8) + ivec2(0, i * this->font->getDimensions().y));
	}
}