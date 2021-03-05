#pragma once

#include "SFMLEngine/EntitySystem/EntitySystem.h"
#include "SFMLEngine/Core/InputManager.h"
#include "Interfaces/EngineTickListener.h"

class Engine
{

public:

	Engine();
	~Engine();

	static Engine* Instance();

	bool Run(sf::Vector2u windowSize, const std::string& name);
	bool IsRunning() const { return m_isRunning; }

	EntitySystem& GetEntitySystem() { return *m_pEntitySystem; }
	InputManager& GetInputManager() { return m_inputManager; }
	sf::RenderWindow& GetMainWindow() { return *m_pMainWindow; }

	void RegisterTickListener(IEngineTickListener& eventListener);
	void UnregisterTickListener(IEngineTickListener& eventListener);

	float GetDeltaSeconds() const { return m_deltaSeconds; }
	sf::Uint32 GetFrameId() const { return m_frameId; }
	sf::Uint32 GetFramesPerSecond() const { return m_lastFpsCounter; }

private:

	Engine(const Engine&) = delete;
	Engine& operator=(Engine const&) = delete;

	void UpdateTitle();

	bool m_isRunning = false;

	std::unique_ptr<sf::RenderWindow> m_pMainWindow;
	std::unique_ptr<EntitySystem> m_pEntitySystem;
	InputManager m_inputManager;

	TEngineTickListeners m_eventListeners;

	float m_deltaSeconds = 0.f;
	sf::Uint32 m_frameId = 0;
	
	sf::Uint32 m_lastFpsCounter = 0;
	sf::Uint32 m_fpsCounter = 0;
	float m_fpsSecondsCounter = 0;

	std::string m_windowTitle;

	static Engine* s_pInstance;

};