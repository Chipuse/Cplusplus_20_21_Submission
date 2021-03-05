#pragma once

#include <vector>

struct IEngineTickListener
{
	virtual void OnStart() { }
	virtual void OnTick(float DeltaSeconds) { }
	virtual void OnStop() { }
};

using TEngineTickListeners = std::vector<IEngineTickListener*>;