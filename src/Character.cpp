#include "Character.hpp"

#include "GameApp.hpp"
#include "Log.hpp"
#include "ResourceManager.hpp"

using namespace Beastmaster;

Character::Character(std::string name, std::string sheetName, Area* area)
{
	this->name = name;
	this->speed = 5.0f;
	this->spriteSheet = ResourceManager::getInstance().getTexture(sheetName);

	this->walkProgress = 0.0f;
	this->direction = ivec2(0, 1);

	this->walkFrame = 0;
	this->walkAnimationSpeed = 10.0f;
	this->walkFrameTimer = 0.0f;

	this->area = area;
}
Character::~Character()
{
}

void Character::update(float deltaTime)
{
	if (this->position != this->nextPosition)
	{
		this->walkProgress += deltaTime * this->speed;
		if (this->walkProgress >= 1.0f)
		{
			this->position = this->nextPosition;
			this->walkProgress = 0.0f;
		}
		this->walkFrameTimer += deltaTime * this->walkAnimationSpeed;
		if (this->walkFrameTimer >= 1.0f)
		{
			this->walkFrameTimer = 0.0f;
			this->walkFrame = (this->walkFrame + 1) % 4;
		}
	}
	else
	{
		this->walkFrameTimer = 0.0f;
		this->walkFrame = 0.0f;
	}
	
	
	ivec2 delta = (this->nextPosition - this->position)*TILE_SIZE;
	this->renderPosition = ivec2(this->position.x*TILE_SIZE + delta.x*this->walkProgress, this->position.y*TILE_SIZE + delta.y*this->walkProgress);
}
void Character::render(ivec2 camera)
{
	ivec2 scale(this->spriteSheet->getWidth() / 4, this->spriteSheet->getHeight() / 4);

	ivec2 offset;
	if (direction == ivec2(1, 0))
		offset.x = scale.x * 3;
	else if (direction == ivec2(-1, 0))
		offset.x = scale.x * 2;
	else if (direction == ivec2(0, 1))
		offset.x = 0;
	else if (direction == ivec2(0, -1))
		offset.x = scale.x;

	offset.y = scale.y * this->walkFrame;

	GameApp::getInstance().renderTexture(this->spriteSheet, (ivec2)this->renderPosition - camera - ivec2(0, scale.y/2), offset, scale);
}

void Character::setPosition(ivec2 position)
{
	this->position = position;
	this->nextPosition = position;
	this->walkProgress = 0.0f;
}

void Character::move(ivec2 direction)
{
	if (this->walkProgress != 0.0f)
		return;

	this->direction = direction;

	if (this->area->getTile(LAYER_COLLISION, this->position + direction) != TILE_WALKABLE)
		return;
	
	this->nextPosition = this->position + direction;
	this->walkProgress = 0.0f;	
}

ivec2 Character::getPosition()
{
	return this->position;
}

ivec2 Character::getNextPosition()
{
	return this->nextPosition;
}

vec2 Character::getRenderPosition()
{
	return this->renderPosition;
}

ivec2 Character::getDirection()
{
	return this->direction;
}

bool Character::isMoving()
{
	if (this->position == this->nextPosition)
		return false;
	return true;
}
