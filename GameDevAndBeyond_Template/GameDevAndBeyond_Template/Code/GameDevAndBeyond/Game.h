#pragma once

#include "SFMLEngine/Core/Interfaces/EngineTickListener.h"
#include "SFMLEngine/Core/InputManager.h"
#include "SFMLEngine/EntitySystem/EntityId.h"
#include "SFMLEngine/Serialization/IArchive.h"

#include "SFMLEngine/FightingEngine/Fighter.h"

class Game 
	: public IEngineTickListener
{

public:

	static Game* Instance();
	
	Game();
	~Game();

private:

	Game(const Game&) = delete;
	Game& operator=(Game const&) = delete;

	virtual void OnStart() override;
	virtual void OnTick(float deltaSeconds) override;

	//void LeftClick();

	//void SaveState();
	//void LoadState();
	void SaveCharacter(Fighter fighter);
	void SaveCharacter(EntityId fighterId);
	Fighter LoadCharacter(std::string fighterName, int playerNum);

	//void Serialize(IArchive& archive);

	const float moveSpeed = 100.0f;

	EntityId m_backgroundEntityId;
	EntityId m_playerOneEntityId;
	EntityId m_playerTwoEntityId;
	EntityId m_musicEntityId;

	InputBindingScope m_inputScope;

	static Game* s_pInstance;
};