#pragma once

#include "SFMLEngine/ComponentSystem/ComponentSystem.h"
#include <array>


struct LogicProperties 
{
	int hitPoints = 0;
	int hitStun = 0;
	float hitStunVelocity = 0;
	sf::Color playerTint = sf::Color(255, 255, 255, 255);
};

class LogicSystem : public ComponentSystemBase
{
public:

	LogicSystem();

	virtual void Update(float deltaSeconds) override;

};