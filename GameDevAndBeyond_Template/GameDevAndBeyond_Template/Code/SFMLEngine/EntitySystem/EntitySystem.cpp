#include "SFMLEngine/Core/Engine.h"
#include "EntitySystem.h"
#include "SFMLEngine/ComponentSystem/ComponentSystem.h"

EntityId EntityId::s_invalidEntityId = EntityId();

//////////////////////////////////////////////////////////////////////////
EntityId EntitySystem::AddEntity()
{
	for (int i = m_nextInsertionIndex; i < m_entities.size(); i++)
	{
		const bool isAlive = m_entities[i].IsAlive();
		if (!isAlive)
		{
			EntityId newId(i, m_entities[i].myId.salt + 1);
			m_entities[i].myId = newId;

			m_nextInsertionIndex = i + 1;
			if (m_nextInsertionIndex >= std::numeric_limits<IndexType>::max())
			{
				//Go back to first valid index
				m_nextInsertionIndex = 1;
			}

			return newId;
		}
	}

	return EntityId();
}

Entity* EntitySystem::GetEntityByID(EntityId id)
{
	if (id.index == 0)
	{
		return nullptr;
	}

	if (m_entities[id.index].IsAlive() && m_entities[id.index].myId.salt == id.salt)
	{
		return &m_entities[id.index];
	}

	return nullptr;
}

bool EntitySystem::DestroyEntity(EntityId id)
{
	Entity* pEntity = GetEntityByID(id);
	if (pEntity != nullptr)
	{
		pEntity->myId.index = 0;
	
		for (int i = 0; i < m_systems.size(); i++)
		{
			m_systems[i]->RemoveEntity(id);
		}
		
		return true;
	}

	return false;
}

EntitySystem& EntitySystem::Instance()
{
	return Engine::Instance()->GetEntitySystem();
}

void EntitySystem::Update(float deltaSeconds)
{
	for (int i = 0; i < m_systems.size(); i++)
	{
		m_systems[i]->Update(deltaSeconds);
	}
}
