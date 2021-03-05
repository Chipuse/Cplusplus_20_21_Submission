#include "Game.h"

#include "SFMLEngine/Core/Engine.h"

#include "SFMLEngine/EntitySystem/EntitySystem.h"
#include "SFMLEngine/ComponentSystem/ComponentSystem.h"

#include "SFMLEngine/GameFramework/Rendering/SpriteRenderSystem.h"
#include "SFMLEngine/GameFramework/Rendering/TextRenderSystem.h"
#include "SFMLEngine/GameFramework/TransformProperties.h"
#include "SFMLEngine/GameFramework/AudioProperties.h"

#include "SFMLEngine/Serialization/BinArchive.h"
#include "SFMLEngine/Serialization/JsonArchive.h"

#include "SFMLEngine/FightingEngine/Lyn.h"
#include "SFMLEngine/FightingEngine/Sailormoon.h"

#include <SFML/Graphics.hpp>

Game* Game::s_pInstance = nullptr;

Game* Game::Instance()
{
	return s_pInstance;
}

Game::Game()
{
	s_pInstance = this;
}

Game::~Game()
{
	if (s_pInstance == this)
	{
		s_pInstance = nullptr;
	}
}

void Game::OnStart()
{
	sf::RenderWindow& window = Engine::Instance()->GetMainWindow();
	EntitySystem& entitySystem = EntitySystem::Instance();

	//Spawn your entities here
	m_backgroundEntityId = entitySystem.AddEntity();
	{
		SpriteRenderProperties* pRenderProperties = entitySystem.AddComponent<SpriteRenderProperties>(m_backgroundEntityId);
		pRenderProperties->texture.loadFromFile("../Assets/Sprites/guileBG.png");
		pRenderProperties->sprite.setTexture(pRenderProperties->texture, true);
		pRenderProperties->visible = true;
		//pRenderProperties->sprite.setScale(2.f, 2.f); //is now dealt by the transform properties
		TransformProperties* pTransformProperties = entitySystem.AddComponent<TransformProperties>(m_backgroundEntityId);
		//pRenderProperties->sprite.setOrigin(pRenderProperties->sprite.getLocalBounds().width / 2 + pRenderProperties->spriteOffset.x, pRenderProperties->sprite.getLocalBounds().height + pRenderProperties->spriteOffset.y);
		//pTransformProperties->SetScale(sf::Vector2f(.3f, .3f));
		pRenderProperties->sprite.setScale(sf::Vector2f(.3f, .3f));
	}


	//initial spawn of characters from file
	LoadCharacter("Lyn", 1);
	LoadCharacter("Moon", 2);

	//old Code to initialize fighters by instanceing from classes
	//Fighter playerOne = Lyn(&entitySystem);
	//SaveCharacter(playerOne);
	//Fighter playerTwo = SailorMoon(&entitySystem);
	//SaveCharacter(playerTwo);
	//m_playerOneEntityId = playerOne.GetMyEntity();
	//m_playerTwoEntityId = playerTwo.GetMyEntity();

	TransformProperties* transfP1 = entitySystem.GetComponent<TransformProperties>(m_playerOneEntityId);
	TransformProperties* transfP2 = entitySystem.GetComponent<TransformProperties>(m_playerTwoEntityId);
	transfP1->SetPosition(sf::Vector2f(-100, 0));
	transfP2->SetPosition(sf::Vector2f(100, 0));
	transfP1->SetTarget(transfP2);
	transfP2->SetTarget(transfP1);
	
	/*
	InputManager& inputManager = Engine::Instance()->GetInputManager();
	inputManager.RegisterKeyboardBinding(sf::Keyboard::S, EInputEventType::Pressed, std::bind(&Game::SaveState, this), m_inputScope);
	inputManager.RegisterKeyboardBinding(sf::Keyboard::L, EInputEventType::Pressed, std::bind(&Game::LoadState, this), m_inputScope);
	inputManager.RegisterMouseBinding(sf::Mouse::Left, EInputEventType::Pressed, std::bind(&Game::LeftClick, this), m_inputScope);
	*/
}

void Game::OnTick(float deltaSeconds)
{
	EntitySystem& entitySystem = EntitySystem::Instance();

	//destroy player 1
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
	{
		entitySystem.DestroyEntity(m_playerOneEntityId);
	}
	//relaod a character for player 1  by filename
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		entitySystem.DestroyEntity(m_playerOneEntityId);
		std::string input;
		std::cin >> input;
		LoadCharacter(input, 1);
	}
	//relaod a character for player 2  by filename
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		entitySystem.DestroyEntity(m_playerTwoEntityId);
		std::string input;
		std::cin >> input;
		LoadCharacter(input, 2);
	}
	//Input for Player 1
	if (AnimatorProperties* animP1 = entitySystem.GetComponent<AnimatorProperties>(m_playerOneEntityId))
	{
		InputProperties* inputP1 = entitySystem.GetComponent<InputProperties>(m_playerOneEntityId);
		sf::Vector2f moveVector;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			moveVector.y += moveSpeed * deltaSeconds;
			inputP1->up.down = true;
		}
		else
			inputP1->up.down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -0.1)
		{
			moveVector.x -= moveSpeed * deltaSeconds;
			inputP1->left.down = true;
		}
		else
			inputP1->left.down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			moveVector.y -= moveSpeed * deltaSeconds;
			inputP1->down.down = true;
		}
		else
			inputP1->down.down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 0.1)
		{
			moveVector.x += moveSpeed * deltaSeconds;
			inputP1->right.down = true;
		}
		else
			inputP1->right.down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0, 0))
		{
			inputP1->actionA.down = true;
		}
		else
			inputP1->actionA.down = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Joystick::isButtonPressed(0, 1))
		{
			inputP1->actionB.down = true;
		}
		else
			inputP1->actionB.down = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		{
			if (LogicProperties* pProperties = entitySystem.GetComponent<LogicProperties>(m_playerOneEntityId))
				pProperties->hitStun = 30;
		}

		//toggle Player 1 Animation and logic update
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			animP1->autoUpdate = !animP1->autoUpdate;
		}
		//let Player 1 do the next Animation and logic step
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
		{
			animP1->advanceStep = true;
		}

		if (TransformProperties* pProperties = entitySystem.GetComponent<TransformProperties>(m_playerOneEntityId))
		{
			if (animP1->activeMove == "idle" && entitySystem.GetComponentSystem<AnimatorSystem>()->GetFreezeFrames() <= 0)
			{
				pProperties->SetPosition(sf::Vector2f(pProperties->GetPosition().x + moveVector.x, pProperties->GetPosition().y));
				//pProperties->position[1] += (moveVector.y);
			}

		}
	}
	//Input for player 2
	if (AnimatorProperties* animP2 = entitySystem.GetComponent<AnimatorProperties>(m_playerTwoEntityId))
	{
		InputProperties* inputP2 = entitySystem.GetComponent<InputProperties>(m_playerTwoEntityId);
		sf::Vector2f moveVector;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			moveVector.y += moveSpeed * deltaSeconds;
			inputP2->up.down = true;
		}
		else
			inputP2->up.down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -0.1)
		{
			moveVector.x -= moveSpeed * deltaSeconds;
			inputP2->left.down = true;
		}
		else
			inputP2->left.down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			moveVector.y -= moveSpeed * deltaSeconds;
			inputP2->down.down = true;
		}
		else
			inputP2->down.down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 0.1)
		{
			moveVector.x += moveSpeed * deltaSeconds;
			inputP2->right.down = true;
		}
		else
			inputP2->right.down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) || sf::Joystick::isButtonPressed(0, 0))
		{
			inputP2->actionA.down = true;
		}
		else
			inputP2->actionA.down = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) || sf::Joystick::isButtonPressed(0, 1))
		{
			inputP2->actionB.down = true;
		}
		else
			inputP2->actionB.down = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			if (LogicProperties* pProperties = entitySystem.GetComponent<LogicProperties>(m_playerTwoEntityId))
				pProperties->hitStun = 30;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
		{
			animP2->autoUpdate = !animP2->autoUpdate;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
		{
			animP2->advanceStep = true;
		}

		if (TransformProperties* pProperties = entitySystem.GetComponent<TransformProperties>(m_playerTwoEntityId))
		{
			if (animP2->activeMove == "idle" && entitySystem.GetComponentSystem<AnimatorSystem>()->GetFreezeFrames() <= 0)
			{
				pProperties->SetPosition(sf::Vector2f(pProperties->GetPosition().x + moveVector.x, pProperties->GetPosition().y));
				//pProperties->position[1] += (moveVector.y);
			}

		}
	}
}

/*
void Game::SaveState()
{
	JsonOutputArchive outputArchive("../Assets/game.dat");
	Serialize(outputArchive);
}

void Game::LoadState()
{
	JsonInputArchive inputArchive("../Assets/game.dat");
	Serialize(inputArchive);
}
*/
//save a character from a Fighter Class
void Game::SaveCharacter(Fighter fighter)
{
	JsonOutputArchive outputArchive("../Assets/" + fighter.name + ".fig");
	fighter.Serialize(outputArchive);
}
// save character by entityID. Releveant for Character Editor
void Game::SaveCharacter(EntityId fighterId)
{
	Fighter temp = Fighter(&EntitySystem::Instance(), fighterId);
	SaveCharacter(temp);
}

//load a character from a file and assign two target player. Character will immediately added to game by initialzing components. Fighter
Fighter Game::LoadCharacter(std::string fighterName, int playernum)
{
	EntitySystem& entitySystem = EntitySystem::Instance();
	JsonInputArchive inputArchive("../Assets/" + fighterName + ".fig");

	Fighter temp = Fighter(&entitySystem);
	temp.Serialize(inputArchive);
	temp.CreateFighter(&entitySystem);
	if (playernum == 1) 
	{
		m_playerOneEntityId = temp.GetMyEntity();
		if (TransformProperties* transfP2 = entitySystem.GetComponent<TransformProperties>(m_playerTwoEntityId))
		{
			TransformProperties* transfP1 = entitySystem.GetComponent<TransformProperties>(m_playerOneEntityId);
			transfP1->SetPosition(sf::Vector2f(-100, 0));
			//transfP2->SetPosition(sf::Vector2f(100, 0));
			transfP1->SetTarget(transfP2);
			transfP2->SetTarget(transfP1);
		}
	}
	else
	{
		m_playerTwoEntityId = temp.GetMyEntity();
		if (TransformProperties* transfP1 = entitySystem.GetComponent<TransformProperties>(m_playerOneEntityId))
		{
			TransformProperties* transfP2 = entitySystem.GetComponent<TransformProperties>(m_playerTwoEntityId);
			transfP2->SetPosition(sf::Vector2f(100, 0));
			transfP1->SetTarget(transfP2);
			transfP2->SetTarget(transfP1);
		}
	}
	return temp;
}

/*
void Game::Serialize(IArchive& archive)
{
	//Save/Load your game state here
}
*/
