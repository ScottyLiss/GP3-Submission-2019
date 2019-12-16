#pragma once
#include "Common.h"
#include "InputState.h"
#define INPUT Input::GetInstance()
class Input
{
private:
	Input();
	static Input* m_instance;
	InputState m_state; // Current input
	InputState m_Old_State; // Old Input from previous frame
public:
	static Input* GetInstance();
	void SetKey(SDL_Keycode key, bool state);
	bool GetKey(SDL_Keycode key); //check if it's currently down or not
	glm::ivec2 GetMousePos() { return m_state.mousePos; }
	void MoveMouse(glm::ivec2 delta) { m_state.oldMousePos = m_state.mousePos; m_state.mousePos += delta; }
	glm::ivec2 GetMouseDelta() { return m_state.mousePos - m_state.oldMousePos; }

	bool GetKeyDown(SDL_Keycode key);
	bool GetKeyUp(SDL_Keycode key);

};