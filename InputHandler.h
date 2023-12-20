#pragma once

#include "SDL.h"
#include <vector>
#include "Vector2D.h"

class InputHandler
{
public:
	static InputHandler* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new InputHandler();
		}
		return s_pInstance;
	}

	void update();
	void clean();

	// keyboard events
	bool isKeyDown(SDL_Scancode key) const;
	bool isKeyUp(SDL_Scancode key) const;

	// joystick events
	float getAxisX(int joy, int stick) const;
	float getAxisY(int joy, int stick) const;

	// init joysticks
	void initialiseJoysticks();
	bool joysticksInitialised() {return m_bJoysticksInitialised;}

private:
	InputHandler();
	~InputHandler();

	static InputHandler* s_pInstance;

	InputHandler(const InputHandler&);
	InputHandler& operator=(const InputHandler&);

	// joystick specific
	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;
	std::vector<SDL_Joystick*> m_joysticks;
	bool m_bJoysticksInitialised;
	const int m_joystickDeadZone = 10000;

	// handle keyboard events
	void onKeyDown(SDL_Event& e);
	void onKeyUp(SDL_Event& e);

	// handle joysticks events
	void onJoystickAxisMove(SDL_Event& event);
};
typedef InputHandler TheInputHandler;