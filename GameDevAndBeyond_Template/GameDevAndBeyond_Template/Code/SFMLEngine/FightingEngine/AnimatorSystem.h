#pragma once

#include "SFMLEngine/EntitySystem/EntitySystem.h"
#include "SFMLEngine/ComponentSystem/ComponentSystem.h"
#include <iostream>
#include <array>
#include <nlohmann/json.hpp>
#include "SFMLEngine/Serialization/IArchive.h"

struct Box
{
	std::vector<int> offset { 0,0 };
	int width = 0;
	int height = 0;
};

struct Hitbox : Box
{
	float damage = 0;
	float hitStun = 0;
	float blockStun = 0;
	int freezeFrames = 100;
	float pushBack = 5;

	void Serialize(IArchive& archive);
};

struct Hurtbox : Box
{
	float armor = 0;

	void Serialize(IArchive& archive);
};

struct SpriteLocation 
{
public:
	int xOffset = 0;
	int yOffset = 0;
	int width = 32;
	int height = 32;

	void Serialize(IArchive& archive);
};

struct ObjectToSpawn
{
	int xOffset = 0;
	int yOffset = 0;
	//kind of object

	void Serialize(IArchive& archive);
};

struct Frame
{
	SpriteLocation spriteLocation;
	std::vector<int> originOffset { 0,0 };
	std::vector<float> transformMove { 0,0 };
	std::vector<Hurtbox> hurtboxes;
	std::vector<Hitbox> hitboxes;
	std::vector<ObjectToSpawn> objects;
	int holdDuration = 10;
	std::vector<EntityId> objectsHit;

	void Serialize(IArchive& archive);
};

struct CancelMove {
	std::string moveName = "";
	int earliestFrame = 0;
	bool onHit = true;

	void Serialize(IArchive& archive);
};

class Move
{
public:
	std::string name;
	std::vector<Frame> frames;
	int activeFrame = 0;
	std::vector<CancelMove> cancelMoves;
	//how to execute the move
	std::vector<int> inputConditions;
	// 0=noButton, 1=aButton, 2=bButton
	int buttonCode = 0;
	bool loop = false;

	virtual void SpecialUpdate() { }
	virtual void OnEnterMove() { }
	virtual void OnExitMove() { }

	void Serialize(IArchive& archive);
};

struct Hit
{
	bool hit = false;
	int frameIndex = 0;
};


struct AnimatorProperties 
{
	std::vector<Move> moves;
	std::string activeMove;
	int holdingFrame = 0;
	Hit hit = { false, 0 };
	bool displayBoxes = true;
	std::string nextMove = "";
	int freezeFramesToAdd = 0;

	bool autoUpdate = true;
	bool advanceStep = false;
};

class AnimatorSystem : public ComponentSystemBase
{
public:

	AnimatorSystem();

	virtual void Update(float deltaSeconds) override;
	int GetFreezeFrames() { return freezeFrames; }
	Move* FindCurrentMove(AnimatorProperties* animProps);
	Move* EngageNewMove(AnimatorProperties* animProps, std::string moveName, Move* lastMove);
	Move* FindMoveByName(AnimatorProperties* animProps, std::string moveName);
private:
	int freezeFrames = 0;


	bool EntityVectorContains(std::vector<EntityId> vec, EntityId number)
	{
		for (int i = 0; i < vec.size(); i++)
		{
			if (vec[i] == number)
				return true;
		}
		return false;
	}

	bool TwoBoxIntersect(Box paramOne, Box paramTwo)
	{
		if (paramOne.offset[0] < paramTwo.offset[0] + paramTwo.width &&
			paramOne.offset[0] + paramOne.width > paramTwo.offset[0] &&
			paramOne.offset[1] < paramTwo.offset[1] + paramTwo.height &&
			paramOne.offset[1] + paramOne.height >  paramTwo.offset[1])
		{
			return true;
		}
		else
		{
			return false;
		}
		return false;
	}

};