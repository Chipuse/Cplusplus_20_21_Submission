#include "EntityId.h"

bool Entity::IsAlive() const
{
	return myId.index != EntityId::s_invalidEntityId.index;
}