#pragma once

#include "Fighter.h"

class Lyn : public Fighter
{
	
public:
	Lyn(EntitySystem* entitySystem) : Fighter(entitySystem)
	{
		name = "Lyn";
		spriteSheet = "../Assets/Sprites/big_Sword.png";
		initialFrame = { 13, 40, 27, 36 };
		spriteFlipped = -1;
		scale = 5.f;
		CreateMoves();
		CreateFighter(entitySystem);
	}
private:
	virtual void CreateMoves() override
	{
		
		//idle Move
		CreateHurtBox(-7,0,15,36,0);
		CreateFrame(13,40,27,36, 10);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(304, 313, 27, 37, 10, 0, -1);

		AddCancelMoveName("forwardDash",0,false);
		AddCancelMoveName("backDash", 0, false);
		AddCancelMoveName("slash", 0, false);
		AddCancelMoveName("uppercut", 0, false);

		CreateMove("idle", 0, true);

		//hitStunMove
		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(411, 248, 36, 29, 1, 2, 0);

		CreateMove("hitStun");

		//slash Move
		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(258, 41, 47, 37, 13, -13, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateHitBox(5, 5, 25, 20, 10, 17, 5, 10, 5);
		CreateFrame(13,91,52,50,7,2,0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(73,107,51,34,20,2,0);

		AddCancelMoveName("uppercut");
		AddCancelMoveName("backDash");
		CreateMove("slash",1);

		//backDash Move	
		CreateFrame(53, 574, 23, 41, 10, 0, 0, -60, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(89, 573, 36, 42, 10, -9, 0, -30, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(127, 589, 36, 26, 20, 0, 0, 0, 0);

		AddInputConditionStep(2);

		AddCancelMoveName("forwardDash", 1, false);

		CreateMove("backDash",2);

		//forwardDash Move
		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(165, 188, 40, 31, 15, 0, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(113, 508, 36, 39, 6, 0, 0,100,0);
		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(113, 508, 36, 39, 6, 0, 0, 30, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(213, 174, 27, 45, 10, 0, 0);

		AddInputConditionStep(1);

		AddCancelMoveName("dashSlash", 2, false);
		
		CreateMove("forwardDash", 2);

		//dash slashMove
		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateHitBox(7, 0, 15, 10, 10, 20, 5, 7, 5);
		CreateFrame(167, 505, 43, 42, 7);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(306, 185, 47, 34, 15, 5, -3);

		AddCancelMoveName("uppercut");
		CreateMove("dashSlash", 1);

		//uppercut Move
		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(73, 107, 51, 34, 7, 2, 0);

		CreateHitBox(7, 0, 15, 70, 50, 50, 15, 15, 35);
		CreateFrame(451, 88, 43, 53, 8, 8, 0);

		//CreateHurtBox(-7, 0, 15, 36, 0);
		//CreateHitBox(0, 0, 35, 70, 25, 20, 10, 7, 10);
		//CreateFrame(451, 88, 43, 53, 15, 9, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(15, 171, 33, 48, 25, -1, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(157, 289, 28, 61, 5, 0, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(199, 297, 41, 53, 5, 7, 0);

		CreateHurtBox(-7, 0, 15, 36, 0);
		CreateFrame(249, 311, 40, 39, 5, 7, 0);

		
		AddInputConditionStep(2);
		AddCancelMoveName("forwardDash");
		
		CreateMove("uppercut", 1);
	}
};