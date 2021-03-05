#pragma once

#include "SFML/Graphics.hpp"

#include <bitset>

using IndexType = sf::Uint16;
using SaltType = sf::Uint16;

struct EntityId
{
	EntityId()
		: index(0)
		, salt(0)
	{ }

	EntityId(IndexType _index, SaltType _salt)
		: index(_index)
		, salt(_salt)
	{ }

	IndexType index;
	SaltType salt;

	bool operator==(const EntityId& rhs) const
	{
		return index == rhs.index && salt == rhs.salt;
	}

	bool operator<(const EntityId& rhs) const
	{
		return index < rhs.index;
	}

	static EntityId s_invalidEntityId;
};

class Entity
{
public:

	std::string name;
	std::bitset<32> addedComponents;
	EntityId myId;

	bool IsAlive() const;

};