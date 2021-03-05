#pragma once

#include "SFMLEngine/EntitySystem/EntityId.h"

class DataComponentIDGeneratorBase
{
protected:

	static int myCounter;
};

template <typename ComponentType>
class DataComponentIDGenerator : public DataComponentIDGeneratorBase
{
public:

	static int GetID()
	{
		static int myId = myCounter++;
		return myId;
	}

	bool operator ==(const DataComponentIDGenerator& rhs) { return GetID() == rhs.GetID(); }
};

class ComponentPoolBase
{
public:

	virtual void AddEntity(EntityId entityId) = 0;
	virtual void* Get(EntityId entityId) = 0;

	virtual ~ComponentPoolBase() {}
};

template<typename PropertyType>
class ComponentPool : public ComponentPoolBase
{
public:

	ComponentPool()
	{
		m_properties.reserve(100);
	}

	virtual void AddEntity(EntityId entityId) override
	{
		if (entityId.index >= m_properties.size())
		{
			m_properties.resize(entityId.index + 1);
			return;
		}

		m_properties[entityId.index] = PropertyType();
	}

	virtual void* Get(EntityId entityId) override
	{
		if (entityId.index >= m_properties.size())
		{
			return nullptr;
		}

		return &m_properties[entityId.index];
	}


private:
	std::vector<PropertyType> m_properties;
};