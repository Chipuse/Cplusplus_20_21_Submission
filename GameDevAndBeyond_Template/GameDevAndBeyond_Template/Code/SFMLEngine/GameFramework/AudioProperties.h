#pragma once

#include "SFML/Audio.hpp"

struct AudioProperties
{
	std::shared_ptr<sf::SoundBuffer> pSoundBuffer;
	std::shared_ptr<sf::Sound> pSound;

	//Constructor
	AudioProperties()
	{ }

	//Copy assignment constructor
	AudioProperties& operator=(const AudioProperties& other)
	{
		pSoundBuffer = other.pSoundBuffer;
		pSound = other.pSound;

		if (pSound && pSoundBuffer)
		{
			pSound->setBuffer(*pSoundBuffer);
		}

		return *this;
	}

	//Copy Constructor
	AudioProperties(const AudioProperties& other)
	{
		pSoundBuffer = other.pSoundBuffer;
		pSound = other.pSound;

		if (pSound && pSoundBuffer)
		{
			pSound->setBuffer(*pSoundBuffer);
		}
	}

	//Move constructor
	AudioProperties(AudioProperties&& other) noexcept
	{
		pSoundBuffer = other.pSoundBuffer;
		pSound = other.pSound;

		if (pSound && pSoundBuffer)
		{
			pSound->setBuffer(*pSoundBuffer);
		}

		other.pSound.reset();
		other.pSoundBuffer.reset();
	}
};