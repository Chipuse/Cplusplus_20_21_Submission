#pragma once

#include "SFMLEngine/EntitySystem/EntitySystem.h"
#include "SFMLEngine/ComponentSystem/ComponentSystem.h"

class TransformSystem : public ComponentSystemBase
{
public:

	TransformSystem();

	virtual void Update(float deltaSeconds) override;

	float GetBoundaries()
	{
		return arenaBoundaries;
	}

private:
	float arenaBoundaries;

};