#include "SFMLEngine/Core/Engine.h"
#include "Game.h"
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>
#include <tinyxml2.h>

#include <future>

int main()
{
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	std::unique_ptr<Game> game = std::make_unique<Game>();

	engine->RegisterTickListener(*game);
	engine->Run(sf::Vector2u(1280, 736), "GameDevAndBeyond");
	//engine->Run(sf::Vector2u(1920, 1080), "GameDevAndBeyond");

	return 0;
}