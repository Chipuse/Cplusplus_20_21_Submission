#pragma once

#include "SFMLEngine/EntitySystem/EntitySystem.h"
#include "SFMLEngine/ComponentSystem/ComponentSystem.h"
#include <array>

struct Direction
{
	//How many Seconds this direction input was alreadyover
	float age = 0;
	/*
	Direction Codes:
	1 = right
	2 = left
	3 = up
	4 = down
	combined directions:
	13 = upright
	14 = downright
	23 = upleft
	24 = downleft

	0 = undefined
	*/
	int diretionCode = 0;
};

//Not necessary. Maybe...
struct BufferedAction
{
	int age = 0;
	//Some ActionCode
};

struct Button 
{
	bool pressed = false;
	bool released = false;
	bool down = false;
};

struct InputProperties
{
	int maxAgeInFrames = 90; 
	std::vector<Direction> directionBuffer;
	Direction currentDirection;

	Button right;
	Button left;
	Button up;
	Button down;
	Button actionA;
	Button actionB;
};


class InputSystem : public ComponentSystemBase
{
public:

	InputSystem();

	virtual void Update(float deltaSeconds) override;

};