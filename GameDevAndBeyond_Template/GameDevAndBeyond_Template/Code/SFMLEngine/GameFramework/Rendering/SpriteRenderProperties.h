#pragma once

#include "SFML/Graphics.hpp"

struct SpriteRenderProperties
{
	bool visible = true;
	int drawOrder = -1;

	sf::Texture texture;
	sf::Sprite sprite;

	sf::Vector2f spriteOffset = sf::Vector2f(0.0f, 0.0f);
	std::vector<sf::RectangleShape> misc;

	//Constructor
	SpriteRenderProperties()
	{ }

	//Copy assignment constructor
	SpriteRenderProperties& operator=(const SpriteRenderProperties& other)
	{
		visible = other.visible;
		drawOrder = other.drawOrder;
		texture = sf::Texture(other.texture);
		sprite = sf::Sprite(other.sprite);

		if (other.sprite.getTexture() == &other.texture)
		{
			sprite.setTexture(texture);
		}

		return *this;
	}

	//Copy Constructor
	SpriteRenderProperties(const SpriteRenderProperties& other)
	{
		visible = other.visible;
		drawOrder = other.drawOrder;
		texture = sf::Texture(other.texture);
		sprite = sf::Sprite(other.sprite);

		if (other.sprite.getTexture() == &other.texture)
		{
			sprite.setTexture(texture);
		}
	}

	//Move constructor
	SpriteRenderProperties(SpriteRenderProperties&& other) noexcept
	{
		visible = other.visible;
		drawOrder = other.drawOrder;
		texture = sf::Texture(other.texture);
		sprite = sf::Sprite(other.sprite);
	
		if (other.sprite.getTexture() == &other.texture)
		{
			sprite.setTexture(texture);
		}

		other.texture = sf::Texture();
		other.sprite = sf::Sprite();
	}
};