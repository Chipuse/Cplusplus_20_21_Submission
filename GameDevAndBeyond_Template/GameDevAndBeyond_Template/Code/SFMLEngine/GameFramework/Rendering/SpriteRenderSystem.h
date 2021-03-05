#pragma once

#include "SFML/Graphics.hpp"
#include "SFMLEngine/ComponentSystem/ComponentSystem.h"
#include "SpriteRenderProperties.h"

#include <array>
#include <thread>
#include <mutex>

struct TransformProperties;
class TextRenderSystem;

enum class ERenderType
{
	Sprite,
	Text
};

struct RenderData
{
	ERenderType renderType = ERenderType::Sprite;

	int drawOrder = -1;
	sf::Sprite sprite;
	sf::Text text;
	sf::Transform transform;
	std::vector<sf::RectangleShape> misc;
};

struct RenderDataQueue
{
	std::vector<RenderData> renderData;
	std::mutex mutex;
};

class SpriteRenderSystem : public ComponentSystemBase
{
public: 

	SpriteRenderSystem();

	void SetTextRenderSystem(TextRenderSystem* pTextRenderSystem);

	virtual void Update(float deltaSeconds) override;

private:

	void RenderThreadLoop();

	std::atomic<int> m_renderThreadQueueIndex = 0;
	std::array<RenderDataQueue, 3> m_renderQueues;
	std::thread m_renderThread;

	TextRenderSystem* m_pTextRenderSystem = nullptr;

};