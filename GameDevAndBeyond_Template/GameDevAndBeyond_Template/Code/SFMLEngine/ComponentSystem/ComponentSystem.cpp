#include "ComponentSystem.h"

int ComponentSystemIDGeneratorBase::myCounter = 0;

void ComponentSystemBase::AddEntity(EntityId entityId)
{
	m_registeredEntities.insert(entityId);
}

void ComponentSystemBase::RemoveEntity(EntityId entityId)
{
	m_registeredEntities.erase(entityId);
}

bool ComponentSystemBase::HasEntity(EntityId entityId)
{
	return m_registeredEntities.find(entityId) != m_registeredEntities.end();
}
