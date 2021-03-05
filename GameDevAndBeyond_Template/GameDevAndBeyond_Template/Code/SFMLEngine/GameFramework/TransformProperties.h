#pragma once

#include "SFML/Graphics.hpp"

struct TransformProperties
{

public:

	void SetPosition(const sf::Vector2f& position);
	void SetRotation(float rotation);
	void SetScale(const sf::Vector2f& scale);

	void SetFacingRight(bool facingRight);
	void SetSpriteFlipped(int spriteFlipped);
	void SetSize(float size);
	void SetWeight(float weight);
	void SetTarget(TransformProperties* target);
	//size
	//weight
	//target

	const sf::Vector2f& GetPosition() const { return m_position; }
	const float GetRotation() const { return m_rotation; }
	const sf::Vector2f& GetScale() const { return m_scale; }

	const bool GetFacingRight() const { return m_facingRight; }
	const int GetSpriteFlipped() const { return m_spriteFlipped; }
	const float GetSize() const { return m_size; }
	const float GetWeight() const { return m_weight; }
	const TransformProperties* GetTarget() const { return m_target; }

	sf::Transform& GetTransform();

private:

	sf::Vector2f m_position;
	sf::Vector2f m_scale = sf::Vector2f(1.f, 1.f);
	float m_rotation = 0.f;

	bool m_facingRight = true;
	int m_spriteFlipped = 1;
	float m_size = 10;
	float m_weight = 1.2;
	TransformProperties* m_target;

	bool m_isDirty = false;
	sf::Transform m_cachedTransform;
};