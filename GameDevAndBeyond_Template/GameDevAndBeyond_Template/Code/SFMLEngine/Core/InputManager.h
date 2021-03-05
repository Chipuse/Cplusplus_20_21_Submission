#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <functional>

enum class EInputBindingType
{
	Keyboard,
	Mouse
};

enum class EInputEventType
{
	Pressed,
	Released
};

struct InputBindingScope
{
	~InputBindingScope();

	struct Scope
	{
		int id = 0;
		EInputBindingType type = EInputBindingType::Keyboard;
	};

	std::vector<Scope> scopes;
};

class InputManager
{
public:

	using InputCallback = std::function<void()>;

	void Update();

	bool RegisterKeyboardBinding(sf::Keyboard::Key key, EInputEventType eventType, const InputCallback& callback, InputBindingScope& scope);
	bool RegisterMouseBinding(sf::Mouse::Button button, EInputEventType eventType, const InputCallback& callback, InputBindingScope& scope);

	void UnregisterBinding(InputBindingScope& scope);

private:

	struct KeyboardBinding
	{
		int id = 0;
		sf::Uint32 lastPressedFrame = 0;
		EInputEventType eventType = EInputEventType::Pressed;
		sf::Keyboard::Key key = sf::Keyboard::Space;
		InputCallback callback;
	};

	struct MouseBinding
	{
		int id = 0;
		sf::Uint32 lastPressedFrame = 0;
		EInputEventType eventType = EInputEventType::Pressed;
		sf::Mouse::Button button = sf::Mouse::Left;
		InputCallback callback;
	};

	int m_inputBindingScopeCounter = 0;
	std::vector<KeyboardBinding> m_keyboardBindings;
	std::vector<MouseBinding> m_mouseBindings;

};