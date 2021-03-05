#include "InputSystem.h"


InputSystem::InputSystem()
{
	AddRequiredComponent<InputProperties>();
}

void InputSystem::Update(float deltaSeconds)
{
	
	EntitySystem& entitySystem = EntitySystem::Instance();
	for (EntityId entityId : m_registeredEntities)
	{
		InputProperties* inputProps = entitySystem.GetComponent<InputProperties>(entityId);

		//Increase age of all buffered Inputs and delete the ones with a too high age
		for (int j = 0; j < inputProps->directionBuffer.size(); j++)
		{
			inputProps->directionBuffer[j].age += 1; //since we are calculöating the age in frames.
			//good ideas someone how i can just delete the ones who are needed to be deleted?
			if (inputProps->directionBuffer[j].age > inputProps->maxAgeInFrames) {
				inputProps->directionBuffer.erase(inputProps->directionBuffer.begin() + j);
				j--;
			}
		}

		//previous direction gets added to Buffer
		if (inputProps->directionBuffer.size() > 0) {
			if (inputProps->currentDirection.diretionCode != -1 && inputProps->currentDirection.diretionCode != inputProps->directionBuffer.back().diretionCode)
				inputProps->directionBuffer.push_back(inputProps->currentDirection);
		}
		else 
		{
			if (inputProps->currentDirection.diretionCode != -1)
				inputProps->directionBuffer.push_back(inputProps->currentDirection);
		}
		if(inputProps->currentDirection.diretionCode != -1 && inputProps->currentDirection.diretionCode != inputProps->directionBuffer.back().diretionCode)
			inputProps->directionBuffer.push_back(inputProps->currentDirection);
		//current direction gets evaluated
		{
			int moveVector[2] = { 0,0 };

			if (inputProps->right.down)
				moveVector[0] += 1;
			if (inputProps->left.down)
				moveVector[0] -= 1;
			if (inputProps->up.down)
				moveVector[1] += 1;
			if (inputProps->down.down)
				moveVector[1] -= 1;

			if (moveVector[0] == 0 && moveVector[1] == 0)
				inputProps->currentDirection.diretionCode = 0;
			else if(moveVector[0] > 0 && moveVector[1] == 0)
				inputProps->currentDirection.diretionCode = 1;
			else if (moveVector[0] < 0 && moveVector[1] == 0)
				inputProps->currentDirection.diretionCode = 2;
			else if (moveVector[0] == 0 && moveVector[1] > 0)
				inputProps->currentDirection.diretionCode = 3;
			else if (moveVector[0] == 0 && moveVector[1] < 0)
				inputProps->currentDirection.diretionCode = 4;
			else if (moveVector[0] > 0 && moveVector[1] > 0)
				inputProps->currentDirection.diretionCode = 13;
			else if (moveVector[0] > 0 && moveVector[1] < 0)
				inputProps->currentDirection.diretionCode = 14;
			else if (moveVector[0] < 0 && moveVector[1] > 0)
				inputProps->currentDirection.diretionCode = 23;
			else if (moveVector[0] < 0 && moveVector[1] < 0)
				inputProps->currentDirection.diretionCode = 24;
		}

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
		
	}
}