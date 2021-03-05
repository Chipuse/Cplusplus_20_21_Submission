#pragma once

#include "TransformProperties.h"

void TransformProperties::SetPosition(const sf::Vector2f& position)
{
	m_position = position;
	m_isDirty = true;
}

void TransformProperties::SetRotation(float rotation)
{
	m_rotation = rotation;
	m_isDirty = true;
}

void TransformProperties::SetScale(const sf::Vector2f& scale)
{
	m_scale = scale;
	m_isDirty = true;
}

void TransformProperties::SetFacingRight(bool facingRight)
{
	m_facingRight = facingRight;
	m_isDirty = true;
}

void TransformProperties::SetSpriteFlipped(int spriteFlipped)
{
	m_spriteFlipped = spriteFlipped;
	m_isDirty = true;
}

void TransformProperties::SetSize(float size)
{
	m_size = size;
	m_isDirty = true;
}

void TransformProperties::SetWeight(float weight)
{
	m_weight = weight;
	m_isDirty = true;
}

void TransformProperties::SetTarget(TransformProperties* target)
{
	m_target = target;
	m_isDirty = true;
}

sf::Transform& TransformProperties::GetTransform()
{
	if (m_isDirty)
	{
		m_cachedTransform = sf::Transform::Identity;
		m_cachedTransform.translate(m_position).rotate(m_rotation).scale(m_scale);
	}

	return m_cachedTransform;
}
