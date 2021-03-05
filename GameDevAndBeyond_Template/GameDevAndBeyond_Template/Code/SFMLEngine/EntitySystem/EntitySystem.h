#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <bitset>

#include "SFMLEngine/ComponentSystem/ComponentPool.h"
#include "SFMLEngine/ComponentSystem/ComponentSystem.h"
#include "EntityId.h"

class EntitySystem
{

public:

	~EntitySystem()
	{
		for (int i = 0; i < m_pools.size(); i++)
		{
			delete m_pools[i];
		}
	}

	static EntitySystem& Instance();

	void Update(float deltaSeconds);

	EntityId AddEntity();
	Entity* GetEntityByID(EntityId id);
	bool DestroyEntity(EntityId id);

	template<typename ComponentSystemType>
	ComponentSystemType& AddSystem();
	template<typename ComponentSystemType>
	ComponentSystemType* GetComponentSystem();

	template <typename ComponentType>
	ComponentType* AddComponent(EntityId entityId)
	{
		Entity* pEntity = GetEntityByID(entityId);
		if (!pEntity)
		{
			return nullptr;
		}

		int componentId = DataComponentIDGenerator<ComponentType>::GetID();

		if (componentId >= m_pools.size())
		{
			m_pools.resize(componentId + 1, nullptr);
			m_pools[componentId] = new ComponentPool<ComponentType>();
		}

		if (!m_pools[componentId])
		{
			m_pools[componentId] = new ComponentPool<ComponentType>();
		}

		if (pEntity->addedComponents.test(componentId))
		{
			return nullptr;
		}

		m_pools[componentId]->AddEntity(entityId);
		pEntity->addedComponents.set(componentId, true);

		for (int i = 0; i < m_systems.size(); i++)
		{
			std::bitset<32>& entityBitset = pEntity->addedComponents;
			const std::bitset<32>& systemBitset = m_systems[i]->GetRequiredComponents();

			bool qualified = (entityBitset & systemBitset) == systemBitset;

			if (qualified && !m_systems[i]->HasEntity(entityId))
			{
				m_systems[i]->AddEntity(entityId);
			}
		}

		return GetComponent<ComponentType>(entityId);
	}
	
	template <typename ComponentType>
	bool RemoveComponent(EntityId entityId)
	{
		Entity* pEntity = GetEntityByID(entityId);
		if (!pEntity)
		{
			return false;
		}

		int componentId = DataComponentIDGenerator<ComponentType>::GetID();

		if (componentId >= m_pools.size())
		{
			return false;
		}

		if (!pEntity->addedComponents.test(componentId))
		{
			return false;
		}

		pEntity->addedComponents.set(componentId, false);

		for (int i = 0; i < m_systems.size(); i++)
		{
			std::bitset<32>& entityBitset = pEntity->addedComponents;
			const std::bitset<32>& systemBitset = m_systems[i]->GetRequiredComponents();

			bool qualified = (entityBitset & systemBitset) == systemBitset;

			if (!qualified && m_systems[i]->HasEntity(entityId))
			{
				m_systems[i]->RemoveEntity(entityId);
			}
		}

		return true;
	}

	template <typename ComponentType>
	bool HasComponent(EntityId entityId)
	{
		return GetComponent<ComponentType>(entityId) != nullptr;
	}

	template <typename ComponentType>
	ComponentType* GetComponent(EntityId entityId)
	{
		Entity* pEntity = GetEntityByID(entityId);
		if (!pEntity)
		{
			return nullptr;
		}

		int componentId = DataComponentIDGenerator<ComponentType>::GetID();
		
		if (componentId >= m_pools.size())
		{
			return nullptr;
		}
		
		if (!pEntity->addedComponents.test(componentId))
		{
			return nullptr;
		}

		void* pAliasedProperty = m_pools[componentId]->Get(entityId);
		ComponentType* pProperty = static_cast<ComponentType*>(pAliasedProperty);

		return pProperty;
	}

private:

	using ComponentSystemBasePtr = std::unique_ptr<ComponentSystemBase>;

	IndexType m_nextInsertionIndex = 1;
	std::array<Entity, std::numeric_limits<IndexType>::max()> m_entities;

	std::vector<ComponentSystemBasePtr> m_systems;
	std::vector<ComponentPoolBase*> m_pools;
};

template<typename ComponentSystemType>
ComponentSystemType& EntitySystem::AddSystem()
{
	static_assert(std::is_base_of<ComponentSystemBase, ComponentSystemType>::value, "Template parameter of this function must derive from ComponentSystemBase!");
	
	std::unique_ptr<ComponentSystemType> pNewSystem = std::make_unique<ComponentSystemType>();
	ComponentSystemType& newSystem = *pNewSystem;
	newSystem.SetTypeID(ComponentSystemIDGenerator<ComponentSystemType>::GetID());

	m_systems.emplace_back(std::move(pNewSystem));

	return newSystem;
}

template<typename ComponentSystemType>
ComponentSystemType* EntitySystem::GetComponentSystem()
{
	static_assert(std::is_base_of<ComponentSystemBase, ComponentSystemType>::value, "Template parameter of this function must derive from ComponentSystemBase!");

	const int searchedID = ComponentSystemIDGenerator<ComponentSystemType>::GetID();
	for (ComponentSystemBasePtr& pSystem : m_systems)
	{
		if (searchedID == pSystem->GetTypeID())
		{
			return static_cast<ComponentSystemType*>(pSystem.get());
		}
	}

	return nullptr;
}
