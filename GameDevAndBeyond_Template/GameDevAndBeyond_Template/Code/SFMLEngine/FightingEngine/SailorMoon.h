#pragma once

#include "Fighter.h"
//used to initialize data once. all Data is also accesible through loading from file

class SailorMoon : public Fighter
{
	
public:
	SailorMoon(EntitySystem* entitySystem) : Fighter(entitySystem)
	{
		name = "Moon";
		spriteSheet = "../Assets/Sprites/prettyMoon.png";
		initialFrame = { 7, 6, 46, 98 };
		spriteFlipped = 1;
		scale = 2.f;
		CreateMoves();
		CreateFighter(entitySystem);
	}
private:
	virtual void CreateMoves() override
	{

		//idle Move
		CreateHurtBox(-22, 0, 45, 100, 0);
		CreateFrame(7, 6, 46, 98, 20);

		//CreateHurtBox(-22, 0, 45, 100, 0);
		//CreateFrame(93, 132, 68, 103, 20, 2, 1);
		//
		//CreateHurtBox(-22, 0, 45, 100, 0);
		//CreateFrame(170, 131, 68, 103, 20, -1, 1);
		//
		//CreateHurtBox(-22, 0, 45, 100, 0);
		//CreateFrame(93, 132, 68, 103, 20, 2, 1);

		AddInputConditionStep(0);
		
		
		AddCancelMoveName("forwardWalk", 0, false);
		AddCancelMoveName("backwardWalk", 0, false);

		CreateMove("idle", 0, true);

		//forwardWalk Move
		{
			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(436, 3, 32, 101, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(478, 4, 31, 100, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(519, 4, 36, 100, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(63, 5, 42, 99, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(115, 4, 38, 100, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(163, 4, 32, 100, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(205, 3, 33, 101, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(248, 4, 32, 100, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(290, 5, 35, 98, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(335, 6, 43, 98, 5, 0, 0, 7);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(386, 5, 39, 99, 5, 0, 0, 7);

			AddInputConditionStep(1);

			AddCancelMoveName("idle", 0, false);
			

			CreateMove("forwardWalk", 0, true);
		}

		//backwardWalk Move
		{
			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(386, 5, 39, 99, 5, 0, 0, -5);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(335, 6, 43, 98, 5, 0, 0, -5);


			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(290, 5, 35, 98, 5, 0, 0, -5);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(248, 4, 32, 100, 5, 0, 0, -5);


			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(205, 3, 33, 101, 5, 0, 0, -5);


			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(163, 4, 32, 100, 5, 0, 0, -5);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(115, 4, 38, 100, 5, 0, 0, -5);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(63, 5, 42, 99, 5, 0, 0, -5);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(519, 4, 36, 100, 5, 0, 0, -5);

			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(478, 4, 31, 100, 5, 0, 0, -5);


			CreateHurtBox(-22, 0, 45, 100, 0);
			CreateFrame(436, 3, 32, 101, 5, 0, 0, -5);


			AddInputConditionStep(2);

			AddCancelMoveName("idle", 0, false);

			CreateMove("backwardWalk", 0, true);
		}
		

		//hitStunMove
		CreateHurtBox(-22, 0, 45, 100, 0);
		CreateFrame(8, 1093, 65, 94, 1, 0, 0);

		CreateMove("hitStun");

		
	}
};