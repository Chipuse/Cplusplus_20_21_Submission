#include "InputManager.h"
#include "SFMLEngine/Core/Engine.h"

InputBindingScope::~InputBindingScope()
{
	Engine::Instance()->GetInputManager().UnregisterBinding(*this);
}

void InputManager::Update()
{
	if (!Engine::Instance()->GetMainWindow().hasFocus())
	{
		return;
	}

	sf::Uint32 frameId = Engine::Instance()->GetFrameId();

	for (KeyboardBinding& binding : m_keyboardBindings)
	{
		if (sf::Keyboard::isKeyPressed(binding.key))
		{
			//Started pressing key
			if (binding.eventType == EInputEventType::Pressed && binding.lastPressedFrame != frameId - 1)
			{
				binding.callback();
			}
			binding.lastPressedFrame = frameId;
		}
		else if (binding.eventType == EInputEventType::Released && binding.lastPressedFrame == frameId - 1)
		{
			//Stopped pressing key
			binding.callback();
		}
	}

	for (MouseBinding& binding : m_mouseBindings)
	{
		if (sf::Mouse::isButtonPressed(binding.button))
		{
			//Started pressing button
			if (binding.eventType == EInputEventType::Pressed && binding.lastPressedFrame != frameId - 1)
			{
				binding.callback();
			}
			binding.lastPressedFrame = frameId;
		}
		else if (binding.eventType == EInputEventType::Released && binding.lastPressedFrame == frameId - 1)
		{
			//Stopped pressing button
			binding.callback();
		}
	}
}

bool InputManager::RegisterKeyboardBinding(sf::Keyboard::Key key, EInputEventType eventType, const InputCallback& callback, InputBindingScope& scope)
{
	KeyboardBinding& newBinding = m_keyboardBindings.emplace_back();
	newBinding.id = ++m_inputBindingScopeCounter;
	newBinding.key = key;
	newBinding.eventType = eventType;
	newBinding.callback = callback;

	InputBindingScope::Scope& newScope = scope.scopes.emplace_back();
	newScope.id = newBinding.id;
	newScope.type = EInputBindingType::Keyboard;

	return true;
}

bool InputManager::RegisterMouseBinding(sf::Mouse::Button button, EInputEventType eventType, const InputCallback& callback, InputBindingScope& scope)
{
	MouseBinding& newBinding = m_mouseBindings.emplace_back();
	newBinding.id = ++m_inputBindingScopeCounter;
	newBinding.button = button;
	newBinding.eventType = eventType;
	newBinding.callback = callback;

	InputBindingScope::Scope& newScope = scope.scopes.emplace_back();
	newScope.id = newBinding.id;
	newScope.type = EInputBindingType::Mouse;

	return true;
}

void InputManager::UnregisterBinding(InputBindingScope& scope)
{
	for (InputBindingScope::Scope& innerScope : scope.scopes)
	{
		switch (innerScope.type)
		{
		case EInputBindingType::Keyboard:
			for (int i = 0; i < m_keyboardBindings.size(); i++)
			{
				if (m_keyboardBindings[i].id == innerScope.id)
				{
					m_keyboardBindings.erase(m_keyboardBindings.begin() + i);
					break;
				}
			}
			break;
		case EInputBindingType::Mouse:
			for (int i = 0; i < m_mouseBindings.size(); i++)
			{
				if (m_mouseBindings[i].id == innerScope.id)
				{
					m_mouseBindings.erase(m_mouseBindings.begin() + i);
					break;
				}
			}
			break;
		}
	}

	scope.scopes.clear();
}
