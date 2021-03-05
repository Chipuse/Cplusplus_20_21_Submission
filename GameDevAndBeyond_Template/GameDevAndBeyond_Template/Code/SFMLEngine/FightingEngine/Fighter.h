#pragma once
#include "SFMLEngine/GameFramework/Rendering/SpriteRenderSystem.h"
#include "SFMLEngine/GameFramework/TransformProperties.h"
#include "InputSystem.h"
#include "AnimatorSystem.h"
#include "LogicSystem.h"
#include <array>
#include "SFML/Graphics.hpp"
#include "SFMLEngine/EntitySystem/EntitySystem.h"

#include <nlohmann/json.hpp>
#include "SFMLEngine/Serialization/IArchive.h"


class  Fighter
{
public:
	Fighter(EntitySystem* entitySystem)
	{
	}
	std::string name = "name";
	std::vector<Move> moves;
	std::string spriteSheet = "";
	SpriteLocation initialFrame = { 7, 6, 46, 98 };
	int spriteFlipped = 1;
	float scale= 2.f;

	EntityId GetMyEntity() {
		return myEntity;
	}
	virtual void CreateMoves() { }

	void Serialize(IArchive& archive);

	void CreateFighter(EntitySystem* entitySystem)
	{
		myEntity = entitySystem->AddEntity();

		SpriteRenderProperties* pRenderProperties = entitySystem->AddComponent<SpriteRenderProperties>(myEntity);
		TransformProperties* pTransformProperties = entitySystem->AddComponent<TransformProperties>(myEntity);
		InputProperties* pInputProperties = entitySystem->AddComponent<InputProperties>(myEntity);
		LogicProperties* pLogicProperties = entitySystem->AddComponent<LogicProperties>(myEntity);
		AnimatorProperties* pAnimatorProperties = entitySystem->AddComponent<AnimatorProperties>(myEntity);
		pAnimatorProperties->activeMove = "idle";

		pRenderProperties->texture.loadFromFile(spriteSheet);
		pRenderProperties->sprite.setTexture(pRenderProperties->texture, true);
		pRenderProperties->sprite.setTextureRect(sf::IntRect(initialFrame.xOffset, initialFrame.yOffset, initialFrame.width, initialFrame.height));
		pRenderProperties->drawOrder = 1;

		pTransformProperties->SetPosition(sf::Vector2f(100, 20));
		pTransformProperties->SetFacingRight(false);
		pTransformProperties->SetSpriteFlipped(spriteFlipped);
		pRenderProperties->sprite.setScale(sf::Vector2f(scale, scale));

		for (int i = 0; i < moves.size(); i++)
		{
			pAnimatorProperties->moves.push_back(moves[i]);
		}
	}

protected:
	std::vector<Frame> frames;

	std::vector<int> inputConditions;

	std::vector<Hitbox> hitboxes;
	std::vector<Hurtbox> hurtboxes;
	std::vector<ObjectToSpawn> objects;
	std::vector<CancelMove> cancelMoves;


	EntityId myEntity;

	void CreateMove(std::string name, int buttonCode = 0, bool loop = false)
	{
		Move returnMove;
		returnMove.name = name;
		returnMove.buttonCode = buttonCode;
		
		returnMove.loop = loop;
		for (int i = 0; i < frames.size(); i++)
		{
			returnMove.frames.push_back(frames[i]);
		}
		frames.clear();
		for (int i = 0; i < inputConditions.size(); i++)
		{
			returnMove.inputConditions.push_back(inputConditions[i]);
		}
		inputConditions.clear();
		for (int i = 0; i < cancelMoves.size(); i++)
		{
			returnMove.cancelMoves.push_back(cancelMoves[i]);
		}
		cancelMoves.clear();
		moves.push_back(returnMove);
	}


	void AddInputConditionStep(int directionCode)
	{
		inputConditions.push_back(directionCode);
	}
	void AddCancelMoveName(std::string moveName,int earliestFrame = 0, bool onHit = true)
	{
		CancelMove returnMove;
		returnMove.moveName = moveName;
		returnMove.earliestFrame = earliestFrame;
		returnMove.onHit = onHit;
		cancelMoves.push_back(returnMove);
	}

	void CreateFrame(int spLoc_xOff, int spLoc_yOff, int spLoc_width, int spLoc_height, int holdDuration, int orig_xOff = 0, int orig_yOff = 0,  float moveX = 0, float moveY = 0) {
		Frame returnFrame;
		returnFrame.spriteLocation.xOffset = spLoc_xOff;
		returnFrame.spriteLocation.yOffset = spLoc_yOff;
		returnFrame.spriteLocation.width = spLoc_width;
		returnFrame.spriteLocation.height = spLoc_height;
		returnFrame.holdDuration = holdDuration;
		returnFrame.originOffset[0] = orig_xOff;
		returnFrame.originOffset[1] = orig_yOff;
		returnFrame.transformMove[0] = moveX;
		returnFrame.transformMove[1] = moveY;
		for (int i = 0; i < hurtboxes.size(); i++)
		{
			returnFrame.hurtboxes.push_back(hurtboxes[i]);
		}
		hurtboxes.clear();
		for (int i = 0; i < hitboxes.size(); i++)
		{
			returnFrame.hitboxes.push_back(hitboxes[i]);
		}
		hitboxes.clear();
		for (int i = 0; i < objects.size(); i++)
		{
			returnFrame.objects.push_back(objects[i]);
		}
		objects.clear();
		frames.push_back(returnFrame);
	}

	void CreateHitBox(int xOff, int yOff, int width, int height, float damage, int hitStun, int blockStun, int freezeFrames, float pushBack) 
	{
		Hitbox returnHitbox;
		returnHitbox.offset[0] = xOff;
		returnHitbox.offset[1] = yOff;
		returnHitbox.width = width;
		returnHitbox.height = height;
		returnHitbox.damage = damage;
		returnHitbox.hitStun = hitStun;
		returnHitbox.blockStun = blockStun;
		returnHitbox.freezeFrames = freezeFrames;
		returnHitbox.pushBack = pushBack;
		hitboxes.push_back(returnHitbox);
	}

	void CreateHurtBox(int xOff, int yOff, int width, int height, float armor)
	{
		Hurtbox returnHitbox;
		returnHitbox.offset[0] = xOff;
		returnHitbox.offset[1] = yOff;
		returnHitbox.width = width;
		returnHitbox.height = height;
		returnHitbox.armor = armor;
		hurtboxes.push_back(returnHitbox);
	}
};

