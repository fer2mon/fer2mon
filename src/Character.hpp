#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <vector>

#include "Area.hpp"
#include "Texture.hpp"
#include "Vec2.hpp"

namespace Beastmaster
{
	class Character
	{
	private:
		std::string name;
		float speed;	//movement speed (interpolation)
		float walkAnimationSpeed;
		
		Texture* spriteSheet;
		Area* area;

		vec2 renderPosition;	//calculated based on integer positions

		ivec2 position;
		ivec2 nextPosition;
		ivec2 direction;
		float walkProgress;		

		int walkFrame;
		float walkFrameTimer;

	public:
		Character(std::string name, std::string sheetName, Area* area);
		~Character();

		void update(float deltaTime);
		void render(ivec2 camera);	

		void move(ivec2 direction);		

		ivec2 getPosition();
		ivec2 getNextPosition();
		vec2 getRenderPosition();
		ivec2 getDirection();

		bool isMoving();

		void setPosition(ivec2 position);
	};
}

#endif