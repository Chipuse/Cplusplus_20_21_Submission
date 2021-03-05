#include "TransformSystem.h"
#include "SFMLEngine/GameFramework/TransformProperties.h"

TransformSystem::TransformSystem() 
{
	arenaBoundaries = 500;
	AddRequiredComponent<TransformProperties>();
}


void TransformSystem::Update(float deltaSeconds)
{
	EntitySystem& entitySystem = EntitySystem::Instance();


	for (EntityId entityId : m_registeredEntities)
	{
		TransformProperties* transformProps = entitySystem.GetComponent<TransformProperties>(entityId);
		if (transformProps->GetPosition().x + transformProps->GetSize() > arenaBoundaries)
			transformProps->SetPosition(sf::Vector2f(arenaBoundaries - transformProps->GetSize(), transformProps->GetPosition().y));
		else if (transformProps->GetPosition().x - transformProps->GetSize() < -arenaBoundaries)
		{
			transformProps->SetPosition(sf::Vector2f(-arenaBoundaries + transformProps->GetSize(), transformProps->GetPosition().y));
		}
	}
}