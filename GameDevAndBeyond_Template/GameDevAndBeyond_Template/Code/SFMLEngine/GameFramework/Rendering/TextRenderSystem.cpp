#include "TextRenderSystem.h"
#include "SFMLEngine/Core/Engine.h"
#include "SFMLEngine/EntitySystem/EntitySystem.h"
#include "SFMLEngine/GameFramework/TransformProperties.h"

TextRenderSystem::TextRenderSystem()
{
	AddRequiredComponent<TextRenderProperties>();
	AddRequiredComponent<TransformProperties>();
}
void TextRenderSystem::AddToRenderData(RenderDataQueue& dataQueue)
{
	EntitySystem& entitySystem = EntitySystem::Instance();

	for (EntityId entityId : m_registeredEntities)
	{
		TextRenderProperties* pTextRenderProperties = entitySystem.GetComponent<TextRenderProperties>(entityId);
		TransformProperties* pTransformProperties = entitySystem.GetComponent<TransformProperties>(entityId);
	
		RenderData& newRenderData = dataQueue.renderData.emplace_back();
		newRenderData.text = pTextRenderProperties->text;
		newRenderData.transform = pTransformProperties->GetTransform();
		newRenderData.renderType = ERenderType::Text;
	}
}