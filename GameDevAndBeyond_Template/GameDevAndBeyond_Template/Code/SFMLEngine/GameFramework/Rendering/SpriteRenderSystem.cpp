#include "SpriteRenderSystem.h"
#include "SFMLEngine/Core/Engine.h"
#include "SFMLEngine/EntitySystem/EntitySystem.h"
#include "SFMLEngine/GameFramework/TransformProperties.h"

#include "SFMLEngine/GameFramework/Rendering/TextRenderSystem.h"

#include <iostream>

SpriteRenderSystem::SpriteRenderSystem()
	: m_renderThread(&SpriteRenderSystem::RenderThreadLoop, this)
{
	AddRequiredComponent<SpriteRenderProperties>();
	AddRequiredComponent<TransformProperties>();

	for (RenderDataQueue& renderDataQueue : m_renderQueues)
	{
		renderDataQueue.renderData.reserve(1000);
	}
}

void SpriteRenderSystem::SetTextRenderSystem(TextRenderSystem* pTextRenderSystem)
{
	m_pTextRenderSystem = pTextRenderSystem;
}

void SpriteRenderSystem::RenderThreadLoop()
{
	sf::RenderWindow& window = Engine::Instance()->GetMainWindow();
	window.setActive(true);

	while (window.isOpen())
	{
		window.clear();

		RenderDataQueue& renderDataQueue = m_renderQueues[m_renderThreadQueueIndex];
		std::lock_guard<std::mutex> lk(renderDataQueue.mutex);

		std::sort(renderDataQueue.renderData.begin(), renderDataQueue.renderData.end(), [](const RenderData& lhs, const RenderData& rhs)
		{
			return lhs.drawOrder < rhs.drawOrder;
		});

		for (const RenderData& renderData : renderDataQueue.renderData)
		{
			if (renderData.renderType == ERenderType::Sprite)
			{
				//Displace all sprites to origin in center middle of screen:
				sf::Transform currentRenderDataTransform = renderData.transform;
				currentRenderDataTransform.translate(sf::Vector2f(window.getSize().x / 2, window.getSize().y));
				window.draw(renderData.sprite, currentRenderDataTransform);
				for (sf::RectangleShape rect : renderData.misc)
				{
					window.draw(rect, currentRenderDataTransform);
				}
			}
			else if (renderData.renderType == ERenderType::Text)
			{
				window.draw(renderData.text, renderData.transform);
			}
		}

		window.display();
	}
}

void SpriteRenderSystem::Update(float deltaSeconds)
{
	EntitySystem& entitySystem = EntitySystem::Instance();

	const int currentRenderThreadIndex = m_renderThreadQueueIndex;
	const int mainThreadIndex = currentRenderThreadIndex + 1 >= m_renderQueues.size() ? 0 : currentRenderThreadIndex + 1;
	
	RenderDataQueue& renderDataQueue = m_renderQueues[mainThreadIndex];
	std::lock_guard<std::mutex> lk(renderDataQueue.mutex);
	
	renderDataQueue.renderData.clear();

	for (EntityId entityId : m_registeredEntities)
	{
		SpriteRenderProperties* pRenderProperties = entitySystem.GetComponent<SpriteRenderProperties>(entityId);

		if (pRenderProperties->visible)
		{
			TransformProperties* pTransformProperties = entitySystem.GetComponent<TransformProperties>(entityId);
			
			RenderData& newRenderData = renderDataQueue.renderData.emplace_back();
			newRenderData.drawOrder = pRenderProperties->drawOrder;
			newRenderData.sprite = pRenderProperties->sprite;
			//setting all special aspects for the sprite frame
			newRenderData.sprite.setOrigin(pRenderProperties->sprite.getLocalBounds().width / 2 + pRenderProperties->spriteOffset.x, pRenderProperties->sprite.getLocalBounds().height + pRenderProperties->spriteOffset.y);
			newRenderData.transform = pTransformProperties->GetTransform();

			if (pTransformProperties->GetFacingRight()) {
				if (pRenderProperties->sprite.getScale().x * pTransformProperties->GetSpriteFlipped() > 0)
					pRenderProperties->sprite.setScale(pRenderProperties->sprite.getScale().x * -1 , pRenderProperties->sprite.getScale().y);
				for (sf::RectangleShape rect : pRenderProperties->misc)
				{
					rect.setScale(rect.getScale().x * -1, rect.getScale().y);
				}
			}
			else {
				if (pRenderProperties->sprite.getScale().x * pTransformProperties->GetSpriteFlipped() < 0)
					pRenderProperties->sprite.setScale(pRenderProperties->sprite.getScale().x * -1 , pRenderProperties->sprite.getScale().y);
				
			}

			newRenderData.misc = pRenderProperties->misc;

			newRenderData.renderType = ERenderType::Sprite;
		}
	}

	m_pTextRenderSystem->AddToRenderData(renderDataQueue);

	m_renderThreadQueueIndex = mainThreadIndex;
}