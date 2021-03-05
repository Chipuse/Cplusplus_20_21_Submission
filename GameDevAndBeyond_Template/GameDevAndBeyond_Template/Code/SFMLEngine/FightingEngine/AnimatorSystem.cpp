#include "AnimatorSystem.h"
#include "InputSystem.h"
#include "SFMLEngine/GameFramework/Rendering/SpriteRenderSystem.h"
#include "SFMLEngine/GameFramework/Rendering/TextRenderSystem.h"
#include "SFMLEngine/GameFramework/TransformProperties.h"
#include "LogicSystem.h"


AnimatorSystem::AnimatorSystem()
{
	AddRequiredComponent<AnimatorProperties>();
	AddRequiredComponent<InputProperties>();
	AddRequiredComponent<SpriteRenderProperties>();
	AddRequiredComponent<TransformProperties>();
	AddRequiredComponent<LogicProperties>();
}

void AnimatorSystem::Update(float deltaSeconds)
{
	EntitySystem& entitySystem = EntitySystem::Instance();
	
	for (EntityId entityId : m_registeredEntities)
	{
		AnimatorProperties* animProps = entitySystem.GetComponent<AnimatorProperties>(entityId);
		InputProperties* inputProps = entitySystem.GetComponent<InputProperties>(entityId);
		SpriteRenderProperties* rendProps = entitySystem.GetComponent<SpriteRenderProperties>(entityId);
		TransformProperties* transfProps = entitySystem.GetComponent<TransformProperties>(entityId);
		LogicProperties* logicProps = entitySystem.GetComponent<LogicProperties>(entityId);
		/*
		if (animProps->autoUpdate != true)
		{
			if (animProps->advanceStep == true)
			{
				animProps->advanceStep = false;
			}
			else
			{
				return;
			}
		}
		*/
		Move* currentMove = AnimatorSystem::FindCurrentMove(animProps);
		if (freezeFrames <= 0) {
			//To play the animation
			animProps->holdingFrame++;
			currentMove->SpecialUpdate();
			if ((currentMove->name != "hitStun" || logicProps->hitStun <= 0) && (animProps->autoUpdate == true || animProps->advanceStep == true))
			{
				animProps->advanceStep = false;

				Move checkedMove;
				int currentMovePrio = 0;
				for (int j = 0; j < currentMove->cancelMoves.size(); j++)
				{
					if (currentMove->cancelMoves[j].onHit == false && currentMove->activeFrame >= currentMove->cancelMoves[j].earliestFrame)
					{
						checkedMove = *FindMoveByName(animProps, currentMove->cancelMoves[j].moveName);
						if (transfProps->GetFacingRight())
						{
							for (int k = 0; k < checkedMove.inputConditions.size(); k++)
							{
								if (checkedMove.inputConditions[k] == 1)
									checkedMove.inputConditions[k] = 2;
								else if(checkedMove.inputConditions[k] == 2)
									checkedMove.inputConditions[k] = 1;
								else if (checkedMove.inputConditions[k] == 13)
									checkedMove.inputConditions[k] = 23;
								else if (checkedMove.inputConditions[k] == 23)
									checkedMove.inputConditions[k] = 13;
								else if (checkedMove.inputConditions[k] == 14)
									checkedMove.inputConditions[k] = 24;
								else if (checkedMove.inputConditions[k] == 24)
									checkedMove.inputConditions[k] = 14;
							}
							
						}
						if (checkedMove.inputConditions.size() <= 0 && currentMovePrio <= 1)
						{
							if (checkedMove.buttonCode == 1 && inputProps->actionA.down)
							{
								animProps->nextMove = currentMove->cancelMoves[j].moveName;
							}
							else if (checkedMove.buttonCode == 2 && inputProps->actionB.down)
							{
								animProps->nextMove = currentMove->cancelMoves[j].moveName;
							}

							else if (checkedMove.buttonCode == 0)
							{
								animProps->nextMove = currentMove->cancelMoves[j].moveName;
							}
						}
						else if (checkedMove.inputConditions.size() == 1 && currentMovePrio <= 2)
						{
							if (inputProps->currentDirection.diretionCode == checkedMove.inputConditions[0])
							{
								if (checkedMove.buttonCode == 1 && inputProps->actionA.down)
								{
									animProps->nextMove = currentMove->cancelMoves[j].moveName;
									currentMovePrio = 2;
								}
								else if (checkedMove.buttonCode == 2 && inputProps->actionB.down)
								{
									animProps->nextMove = currentMove->cancelMoves[j].moveName;
									currentMovePrio = 2;
								}
								else if (checkedMove.buttonCode == 0 && currentMovePrio == 0)
								{
									animProps->nextMove = currentMove->cancelMoves[j].moveName;
									currentMovePrio = 0;
								}
							}
							

							//
						}
						else if (checkedMove.inputConditions.size() > 1 && currentMovePrio <= checkedMove.inputConditions.size()+1)
						{
							bool correctInput = false;
							int inputCounter = 0;
							std::cout << "\n";
							for (int k = 0; k < inputProps->directionBuffer.size(); k++ )
							{
								std::cout << inputProps->directionBuffer[k].diretionCode << ":"<< inputProps->directionBuffer[k].age << " | ";
								if (inputProps->directionBuffer[k].diretionCode == checkedMove.inputConditions[inputCounter])
								{
									inputCounter++;
								}
								if (inputCounter >= checkedMove.inputConditions.size())
								{
									correctInput = true;
									k = inputProps->directionBuffer.size();
									
								}
							}

							if (checkedMove.buttonCode == 1 && inputProps->actionA.down && correctInput)
							{
								animProps->nextMove = currentMove->cancelMoves[j].moveName;
								currentMovePrio = checkedMove.inputConditions.size()+1;
								inputProps->directionBuffer.clear();
							}
							else if (checkedMove.buttonCode == 2 && inputProps->actionB.down && correctInput)
							{
								animProps->nextMove = currentMove->cancelMoves[j].moveName;
								currentMovePrio = checkedMove.inputConditions.size()+1;
								inputProps->directionBuffer.clear();
							}
							else if (checkedMove.buttonCode == 0 && correctInput)
							{
								animProps->nextMove = currentMove->cancelMoves[j].moveName;
								currentMovePrio = checkedMove.inputConditions.size() + 1;
								inputProps->directionBuffer.clear();
							}

							//currentMovePrio = checkedMove.inputConditions.size();
						}
					}
					
				}
				
				if (animProps->hit.hit)
				{
					for (int j = 0; j < currentMove->cancelMoves.size(); j++)
					{
						if (animProps->hit.frameIndex != currentMove->activeFrame)
						{
							if (currentMove->cancelMoves[j].onHit == true && currentMove->activeFrame >= currentMove->cancelMoves[j].earliestFrame)
							{
								checkedMove = *FindMoveByName(animProps, currentMove->cancelMoves[j].moveName);
								if (transfProps->GetFacingRight())
								{
									for (int k = 0; k < checkedMove.inputConditions.size(); k++)
									{
										if (checkedMove.inputConditions[k] == 1)
											checkedMove.inputConditions[k] = 2;
										else if (checkedMove.inputConditions[k] == 2)
											checkedMove.inputConditions[k] = 1;
										else if (checkedMove.inputConditions[k] == 13)
											checkedMove.inputConditions[k] = 23;
										else if (checkedMove.inputConditions[k] == 23)
											checkedMove.inputConditions[k] = 13;
										else if (checkedMove.inputConditions[k] == 14)
											checkedMove.inputConditions[k] = 24;
										else if (checkedMove.inputConditions[k] == 24)
											checkedMove.inputConditions[k] = 14;
									}

								}
								if (checkedMove.inputConditions.size() <= 0 && currentMovePrio == 0)
								{
									if (checkedMove.buttonCode == 1 && inputProps->actionA.down)
									{
										animProps->nextMove = currentMove->cancelMoves[j].moveName;
									}
									else if (checkedMove.buttonCode == 2 && inputProps->actionB.down)
									{
										animProps->nextMove = currentMove->cancelMoves[j].moveName;
									}
								}
								else if (checkedMove.inputConditions.size() == 1 && currentMovePrio <= 1)
								{
									if (inputProps->currentDirection.diretionCode == checkedMove.inputConditions[0])
									{
										if (checkedMove.buttonCode == 1 && inputProps->actionA.down)
										{
											animProps->nextMove = currentMove->cancelMoves[j].moveName;
											currentMovePrio = 1;
										}
										else if (checkedMove.buttonCode == 2 && inputProps->actionB.down)
										{
											animProps->nextMove = currentMove->cancelMoves[j].moveName;
											currentMovePrio = 1;
										}
									}


									//
								}
								else if (checkedMove.inputConditions.size() > 1 && currentMovePrio <= checkedMove.inputConditions.size())
								{
									if (checkedMove.buttonCode == 1 && inputProps->actionA.down)
									{
										animProps->nextMove = currentMove->cancelMoves[j].moveName;
									}
									else if (checkedMove.buttonCode == 2 && inputProps->actionB.down)
									{
										animProps->nextMove = currentMove->cancelMoves[j].moveName;
									}

									//currentMovePrio = checkedMove.inputConditions.size();
								}
							}
						}
					}
				}
				
				
				if (animProps->holdingFrame > currentMove->frames[currentMove->activeFrame].holdDuration)
				{
					if (currentMove->frames.size() <= currentMove->activeFrame + 1)
					{
						if (currentMove->loop)
						{
							currentMove->activeFrame = 0;
							animProps->holdingFrame = 0;
						}
						else
						{
							animProps->nextMove = "idle";
							rendProps->spriteOffset = sf::Vector2f(currentMove->frames[currentMove->activeFrame].originOffset[0], currentMove->frames[currentMove->activeFrame].originOffset[1]);
						}
					}
					else
					{
						currentMove->activeFrame++;

						animProps->holdingFrame = 0;
					}

				}
			}

			//Check for collisions between hitboxes and other objects hurtboxes

			rendProps->misc.clear();
			if (currentMove->frames[currentMove->activeFrame].hitboxes.size() > 0)
			{
				for (EntityId otherEntityId : m_registeredEntities)
				{
					if (!EntityVectorContains(currentMove->frames[currentMove->activeFrame].objectsHit, otherEntityId))
					{
						if (otherEntityId.index != entityId.index)
						{
							AnimatorProperties* animPropsOther = entitySystem.GetComponent<AnimatorProperties>(otherEntityId);
							SpriteRenderProperties* rendPropsOther = entitySystem.GetComponent<SpriteRenderProperties>(otherEntityId);
							TransformProperties* transfPropsOther = entitySystem.GetComponent<TransformProperties>(otherEntityId);
							LogicProperties* logicPropsOther = entitySystem.GetComponent<LogicProperties>(otherEntityId);
							Move* currentMoveOther = AnimatorSystem::FindCurrentMove(animPropsOther);

							if (currentMoveOther->frames[currentMoveOther->activeFrame].hurtboxes.size() > 0)
							{
								for (int l = 0; l < currentMove->frames[currentMove->activeFrame].hitboxes.size(); l++)
								{
									for (int k = 0; k < currentMoveOther->frames[currentMoveOther->activeFrame].hurtboxes.size(); k++)
									{
										Hitbox hitBox = currentMove->frames[currentMove->activeFrame].hitboxes[l];
										int facingMult = -1;
										if (!transfProps->GetFacingRight())
											facingMult = 1;
										if (facingMult == 1)
											hitBox.offset[0] = hitBox.offset[0] * facingMult * rendProps->sprite.getScale().y + transfProps->GetPosition().x;
										else
										{
											hitBox.offset[0] = (hitBox.offset[0] + hitBox.width) * facingMult * rendProps->sprite.getScale().y + transfProps->GetPosition().x;
										}

										hitBox.offset[1] = hitBox.offset[1] * rendProps->sprite.getScale().y + (transfProps->GetPosition().y);
										hitBox.width = hitBox.width * rendProps->sprite.getScale().y;
										hitBox.height = hitBox.height  * rendProps->sprite.getScale().y;

										Hurtbox hurtBox = currentMoveOther->frames[currentMoveOther->activeFrame].hurtboxes[k];
										facingMult = -1;
										if (!transfPropsOther->GetFacingRight())
											facingMult = 1;
										if (facingMult == 1)
											hurtBox.offset[0] = hurtBox.offset[0] * facingMult * rendPropsOther->sprite.getScale().y + transfPropsOther->GetPosition().x;
										else
										{
											hurtBox.offset[0] = (hurtBox.offset[0] + hurtBox.width) * facingMult * rendPropsOther->sprite.getScale().y + transfPropsOther->GetPosition().x;
										}
										hurtBox.offset[1] = hurtBox.offset[1] * rendPropsOther->sprite.getScale().y + (transfPropsOther->GetPosition().y);
										hurtBox.width = hurtBox.width * rendPropsOther->sprite.getScale().y;
										hurtBox.height = hurtBox.height  * transfPropsOther->GetScale().y;

										if (TwoBoxIntersect(hitBox, hurtBox))
										{
											logicPropsOther->hitStun = currentMove->frames[currentMove->activeFrame].hitboxes[l].hitStun;
											if (!transfProps->GetFacingRight())
												logicPropsOther->hitStunVelocity = currentMove->frames[currentMove->activeFrame].hitboxes[l].pushBack;
											else
												logicPropsOther->hitStunVelocity = currentMove->frames[currentMove->activeFrame].hitboxes[l].pushBack * -1;
											currentMove->frames[currentMove->activeFrame].objectsHit.push_back(otherEntityId);
											animProps->freezeFramesToAdd = currentMove->frames[currentMove->activeFrame].hitboxes[l].freezeFrames;
											animProps->hit.hit = true;
											animProps->hit.frameIndex = currentMove->activeFrame;
										}
									}
								}
							}
						}
					}

				}
				if (animProps->displayBoxes)
				{
					for (int j = 0; j < currentMove->frames[currentMove->activeFrame].hitboxes.size(); j++)
					{
						sf::RectangleShape rect;

						rect.setFillColor(sf::Color(255, 0, 0, 20));
						rect.setOutlineColor(sf::Color(255, 0, 0, 255));
						rect.setOutlineThickness(2);
						int facingMult = -1;
						if (!transfProps->GetFacingRight())
							facingMult = 1;
						rect.setPosition(sf::Vector2f(
							currentMove->frames[currentMove->activeFrame].hitboxes[j].offset[0] * facingMult * rendProps->sprite.getScale().y,
							currentMove->frames[currentMove->activeFrame].hitboxes[j].offset[1] * -1 * rendProps->sprite.getScale().y));
						rect.setSize(sf::Vector2f(
							currentMove->frames[currentMove->activeFrame].hitboxes[j].width * facingMult * rendProps->sprite.getScale().y,
							currentMove->frames[currentMove->activeFrame].hitboxes[j].height * -1 * rendProps->sprite.getScale().y));
						rendProps->misc.push_back(rect);

					}
				}


			}

			if (animProps->displayBoxes)
			{
				for (int j = 0; j < currentMove->frames[currentMove->activeFrame].hurtboxes.size(); j++)
				{
					sf::RectangleShape rect;
					rect.setFillColor(sf::Color(0, 255, 0, 20));
					rect.setOutlineColor(sf::Color(0, 255, 0, 255));
					rect.setOutlineThickness(2);
					int facingMult = -1;
					if (!transfProps->GetFacingRight())
						facingMult = 1;
					rect.setPosition(sf::Vector2f(
						currentMove->frames[currentMove->activeFrame].hurtboxes[j].offset[0] * facingMult * rendProps->sprite.getScale().x,
						currentMove->frames[currentMove->activeFrame].hurtboxes[j].offset[1] * -1 * rendProps->sprite.getScale().y));
					rect.setSize(sf::Vector2f(
						currentMove->frames[currentMove->activeFrame].hurtboxes[j].width * facingMult * rendProps->sprite.getScale().x,
						currentMove->frames[currentMove->activeFrame].hurtboxes[j].height * -1 * rendProps->sprite.getScale().y));
					rendProps->misc.push_back(rect);
				}
			}
			int facingMult = -1;
			if (!transfProps->GetFacingRight())
				facingMult = 1;
			transfProps->SetPosition(sf::Vector2f(
				transfProps->GetPosition().x + currentMove->frames[currentMove->activeFrame].transformMove[0] * facingMult / currentMove->frames[currentMove->activeFrame].holdDuration, 
				transfProps->GetPosition().y + currentMove->frames[currentMove->activeFrame].transformMove[1] / currentMove->frames[currentMove->activeFrame].holdDuration				
				));
			rendProps->sprite.setColor(logicProps->playerTint);
		}
		else
		{
			rendProps->sprite.setColor( sf::Color(0, 0, 0, 255));
		}

			

		
		//Display the right part of the spritesheet
		rendProps->spriteOffset = sf::Vector2f(currentMove->frames[currentMove->activeFrame].originOffset[0], currentMove->frames[currentMove->activeFrame].originOffset[1]);
		rendProps->sprite.setTextureRect(sf::IntRect(
			currentMove->frames[currentMove->activeFrame].spriteLocation.xOffset,
			currentMove->frames[currentMove->activeFrame].spriteLocation.yOffset,
			currentMove->frames[currentMove->activeFrame].spriteLocation.width,
			currentMove->frames[currentMove->activeFrame].spriteLocation.height
		));
		
	}
	for (EntityId entityId : m_registeredEntities)
	{
		AnimatorProperties* animProps = entitySystem.GetComponent<AnimatorProperties>(entityId);
		if (animProps->freezeFramesToAdd >= freezeFrames)
			freezeFrames = animProps->freezeFramesToAdd;
		animProps->freezeFramesToAdd = 0;
	}

	if (freezeFrames <= 0) {

	}
	else
	{
		freezeFrames--;
	}

	if (freezeFrames <= 0)
	{
		for (EntityId entityId : m_registeredEntities)
		{
			AnimatorProperties* animProps = entitySystem.GetComponent<AnimatorProperties>(entityId);
			InputProperties* inputProps = entitySystem.GetComponent<InputProperties>(entityId);
			SpriteRenderProperties* rendProps = entitySystem.GetComponent<SpriteRenderProperties>(entityId);
			TransformProperties* transfProps = entitySystem.GetComponent<TransformProperties>(entityId);
			LogicProperties* logicProps = entitySystem.GetComponent<LogicProperties>(entityId);

			Move* currentMove = AnimatorSystem::FindCurrentMove(animProps);
			if (logicProps->hitStun > 0)
			{
				if (currentMove->name == "hitStun")
				{
					transfProps->SetPosition(sf::Vector2f(
						transfProps->GetPosition().x + logicProps->hitStunVelocity,
						transfProps->GetPosition().y
					));
					logicProps->hitStunVelocity /= transfProps->GetWeight();
					logicProps->hitStun--;
				}
				else
				{
					animProps->nextMove = "hitStun";
				}
			}

			if (animProps->nextMove != "")
			{
				if (transfProps->GetTarget()->GetPosition().x > transfProps->GetPosition().x)
					transfProps->SetFacingRight(false);
				else
					transfProps->SetFacingRight(true);

				currentMove->OnExitMove();
				EngageNewMove(animProps, animProps->nextMove, currentMove);
				currentMove->OnEnterMove();
			}
			if (animProps->activeMove == "idle")
			{
				if (transfProps->GetTarget()->GetPosition().x > transfProps->GetPosition().x)
					transfProps->SetFacingRight(false);
				else
					transfProps->SetFacingRight(true);

			}

		}
	}
	
	
}

Move* AnimatorSystem::FindCurrentMove(AnimatorProperties* animProps)
{
	Move* returnMove;
	for (int i = 0; i < animProps->moves.size(); i++)
	{
		if (animProps->activeMove == animProps->moves[i].name)
		{
			returnMove = &animProps->moves[i];
			return returnMove;
		}
	}
	return nullptr;
}

Move* AnimatorSystem::FindMoveByName(AnimatorProperties* animProps, std::string moveName)
{
	Move* returnMove;
	for (int i = 0; i < animProps->moves.size(); i++)
	{
		if (moveName == animProps->moves[i].name)
		{
			returnMove = &animProps->moves[i];
			return returnMove;
		}
	}
	return nullptr;
}

Move* AnimatorSystem::EngageNewMove(AnimatorProperties* animProps, std::string moveName, Move* lastMove)
{
	Move* returnMove;
	
	for (int i = 0; i < lastMove->frames.size(); i++)
	{
		lastMove->frames[i].objectsHit.clear();
	}

	animProps->holdingFrame = 0;
	animProps->activeMove = moveName;
	animProps->nextMove = "";
	animProps->hit.hit = false;
	for (int i = 0; i < animProps->moves.size(); i++)
	{
		if (moveName == animProps->moves[i].name)
		{
			returnMove = &animProps->moves[i];
			//returnMove->frames[FindCurrentMove(animProps)->activeFrame].objectsHit.clear();
			returnMove->activeFrame = 0;
			return returnMove;
		}
	}
	return nullptr;
}

//Serialization defines for all the different structs

void Hitbox::Serialize(IArchive& archive)
{
	archive.Serialize(offset, "offset");
	archive.Serialize(width, "width");
	archive.Serialize(height, "height");

	archive.Serialize(damage, "damage");
	archive.Serialize(hitStun, "hitStun");
	archive.Serialize(blockStun, "blockStun");
	archive.Serialize(freezeFrames, "freezeFrames");
	archive.Serialize(pushBack, "pushBack");
}

void Hurtbox::Serialize(IArchive& archive)
{
	archive.Serialize(offset, "offset");
	archive.Serialize(width, "width");
	archive.Serialize(height, "height");

	archive.Serialize(armor, "armor");
}

void SpriteLocation::Serialize(IArchive& archive)
{
	archive.Serialize(xOffset, "xOffset");
	archive.Serialize(yOffset, "yOffset");
	archive.Serialize(width, "width");
	archive.Serialize(height, "height");
}

void ObjectToSpawn::Serialize(IArchive& archive)
{
	archive.Serialize(xOffset, "xOffset");
	archive.Serialize(yOffset, "yOffset");
}

void Frame::Serialize(IArchive& archive)
{
	archive.Serialize(spriteLocation, "spriteLocation");
	archive.Serialize(originOffset, "originOffset");
	archive.Serialize(transformMove, "transformMove");
	archive.Serialize(hurtboxes, "hurtboxes");
	archive.Serialize(hitboxes, "hitboxes");
	archive.Serialize(objects, "objects");
	archive.Serialize(holdDuration, "holdDuration");
	//archive.Serialize(objectsHit, "objectsHit");	
}

void Move::Serialize(IArchive& archive)
{
	archive.Serialize(name, "name");
	archive.Serialize(frames, "frames");
	archive.Serialize(activeFrame, "activeFrame");
	archive.Serialize(cancelMoves, "cancelMoves");
	archive.Serialize(inputConditions, "inputConditions");
	archive.Serialize(buttonCode, "buttonCode");
	archive.Serialize(loop, "loop");
}

void CancelMove::Serialize(IArchive& archive)
{
	archive.Serialize(moveName, "moveName");
	archive.Serialize(earliestFrame, "earliestFrame");
	archive.Serialize(onHit, "onHit");
}

//Serialization end