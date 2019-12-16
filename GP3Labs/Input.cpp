#include "pch.h"
#include "Input.h"
Input* Input::m_instance = nullptr;
Input::Input()
{
}
Input* Input::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Input();
	}
	return m_instance;
}
void Input::SetKey(SDL_Keycode key, bool state)
{
	int index = key;
	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}



	if (index < m_state.keys.size())
	{
		m_Old_State.keys[index] = m_state.keys[index];
		m_state.keys[index] = state;
	}
}
bool Input::GetKey(SDL_Keycode key)
{
	int index = key;
	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}
	if (index < m_state.keys.size())
	{
		return m_state.keys[index];
	}
	else return false;
}

bool Input::GetKeyDown(SDL_Keycode key)
{
	int index = key;

	if (key >= SDLK_CAPSLOCK)
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}

	if (index < m_state.keys.size())
	{
		if (m_Old_State.keys[index])
		{
			return false;
		}
		else return true;
	}
}

bool Input::GetKeyUp(SDL_Keycode key)
{

	int index = key;

	if (key >= SDLK_CAPSLOCK) // Checking if capslock has been pressed to ensure we provide the right keycode
	{
		index = (key - SDLK_SCANCODE_MASK) + 128;
	}

	if (index < m_state.keys.size()) // Check if index is less than the size of keys, to ensure it is there
	{
		if (m_Old_State.keys[index]) // Check if the key is active or not 
		{
			return true; // Return if active (i.e down, the value is 1)
		}
		else return false;
	}
	
}
