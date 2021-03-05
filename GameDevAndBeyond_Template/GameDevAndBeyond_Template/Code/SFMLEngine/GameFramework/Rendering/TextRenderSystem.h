#pragma once

#include "SFML/Graphics.hpp"
#include "SFMLEngine/ComponentSystem/ComponentSystem.h"
#include "SFMLEngine/GameFramework/Rendering/SpriteRenderSystem.h"
#include "TextRenderProperties.h"

class TextRenderSystem : public ComponentSystemBase
{
public: 

	TextRenderSystem();

	void AddToRenderData(RenderDataQueue& dataQueue);

};