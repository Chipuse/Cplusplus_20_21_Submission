#include "Engine.h"
#include "Interfaces/EngineTickListener.h"
#include "SFMLEngine/GameFramework/Rendering/SpriteRenderSystem.h"
#include "SFMLEngine/GameFramework/Rendering/TextRenderSystem.h"
#include "SFMLEngine/FightingEngine/AnimatorSystem.h"
#include "SFMLEngine/FightingEngine/InputSystem.h"
#include "SFMLEngine/FightingEngine/LogicSystem.h"
#include "SFMLEngine/FightingEngine/TransformSystem.h"

#include <iostream>
#include <sstream>

Engine* Engine::s_pInstance = nullptr;

Engine* Engine::Instance()
{
	return s_pInstance;
}

//////////////////////////////////////////////////////////////////////////
Engine::Engine()
{
	s_pInstance = this;
}

Engine::~Engine()
{
	if (s_pInstance == this)
	{
		s_pInstance = nullptr;
	}
}

bool Engine::Run(sf::Vector2u windowSize, const std::string& name)
{
	if (IsRunning())
	{
		return false;
	}

	m_windowTitle = name;
	m_pMainWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(windowSize.x, windowSize.y), m_windowTitle);
	m_pEntitySystem = std::make_unique<EntitySystem>();
	
	m_pMainWindow->setFramerateLimit(30);
	m_pMainWindow->setActive(false);

	UpdateTitle();

	//Setting up all the systems
	SpriteRenderSystem& spriteRenderSystem = m_pEntitySystem->AddSystem<SpriteRenderSystem>();
	TextRenderSystem& textRenderSystem = m_pEntitySystem->AddSystem<TextRenderSystem>();
	spriteRenderSystem.SetTextRenderSystem(&textRenderSystem);

	AnimatorSystem& animSystem = m_pEntitySystem->AddSystem<AnimatorSystem>();
	InputSystem& inputSystem = m_pEntitySystem->AddSystem<InputSystem>();
	TransformSystem& transformSystem = m_pEntitySystem->AddSystem<TransformSystem>();

	for (IEngineTickListener* pEventListener : m_eventListeners)
	{
		pEventListener->OnStart();
	}

	sf::Clock clock;
	sf::Time lastFrameTime;

	while (m_pMainWindow->isOpen())
	{
		//Game Loop
		sf::Time currentTime = clock.getElapsedTime();
	
		m_deltaSeconds = (currentTime - lastFrameTime).asMicroseconds() / 1000.0f / 1000.0f;
		m_frameId++;

		m_fpsSecondsCounter += m_deltaSeconds;
		if (m_fpsSecondsCounter >= 1.f)
		{
			m_fpsSecondsCounter -= 1.f;
			m_lastFpsCounter = m_fpsCounter;
			m_fpsCounter = 0;

			UpdateTitle();
		}
		else
		{
			m_fpsCounter++;
		}

		sf::Event event;
		while (m_pMainWindow->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				m_pMainWindow->close();
			}
		}

		m_pEntitySystem->Update(m_deltaSeconds);
		m_inputManager.Update();

		for (IEngineTickListener* pEngineTickListener : m_eventListeners)
		{
			pEngineTickListener->OnTick(m_deltaSeconds);
		}

		lastFrameTime = currentTime;
	}

	for (IEngineTickListener* pEngineTickListener : m_eventListeners)
	{
		pEngineTickListener->OnStop();
	}

	m_pEntitySystem.release();
	return true;
}

void Engine::RegisterTickListener(IEngineTickListener& eventListener)
{
	m_eventListeners.push_back(&eventListener);
}

void Engine::UnregisterTickListener(IEngineTickListener& eventListener)
{
	m_eventListeners.erase(std::remove(m_eventListeners.begin(), m_eventListeners.end(), &eventListener), m_eventListeners.end());
}

void Engine::UpdateTitle()
{
	std::ostringstream titleStream;
	titleStream << 1.f / m_deltaSeconds;//m_windowTitle << " (" << m_lastFpsCounter << ")";
	m_pMainWindow->setTitle(titleStream.str());
}

