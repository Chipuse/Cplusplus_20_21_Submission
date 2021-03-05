#pragma once

#include <vector>
#include <set>
#include <bitset>

#include "SFMLEngine/EntitySystem/EntityId.h"
#include "ComponentPool.h"

class ComponentSystemIDGeneratorBase
{
protected:

	static int myCounter;
};

template <typename ComponentType>
class ComponentSystemIDGenerator : public ComponentSystemIDGeneratorBase
{
public:

	static int GetID()
	{
		static int myId = myCounter++;
		return myId;
	}

	bool operator ==(const ComponentSystemIDGenerator& rhs) { return GetID() == rhs.GetID(); }
};

class ComponentSystemBase
{
public:

	int GetTypeID() const { return m_typeId; }
	void SetTypeID(int typeId) { m_typeId = typeId; }

	void AddEntity(EntityId entityId);
	void RemoveEntity(EntityId entityId);
	bool HasEntity(EntityId entityId);

	virtual void Update(float deltaSeconds) { };

	const std::bitset<32>& GetRequiredComponents() const 
	{ 
		return m_requiredComponents; 
	}

protected:

	template <typename ComponentType>
	void AddRequiredComponent()
	{
		int componentId = DataComponentIDGenerator<ComponentType>::GetID();
		m_requiredComponents.set(componentId, true);
	}

	int m_typeId;
	std::bitset<32> m_requiredComponents;
	std::set<EntityId> m_registeredEntities;
};