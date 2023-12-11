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

	// joystick events
	int getAxisX(int joy, int stick) const;
	int getAxisY(int joy, int stick) const;
	bool getButtonState(int joy, int buttonNumber) const;

	// init joysticks
	void initialiseJoysticks();
	bool joysticksInitialised() {return m_bJoysticksInitialised;}

private:
	InputHandler();
	~InputHandler();

	static InputHandler* s_pInstance;

	InputHandler(const InputHandler&);
	InputHandler& operator=(const InputHandler&);

	// keyboard specific
	const Uint8* m_keystates;

	// joystick specific
	std::vector<std::pair<Vector2D*, Vector2D*>> m_joystickValues;
	std::vector<SDL_Joystick*> m_joysticks;
	std::vector<std::vector<bool>> m_buttonStates;
	bool m_bJoysticksInitialised;
	static const int m_joystickDeadZone = 10000;

	// handle keyboard events
	void onKeyDown();
	void onKeyUp();

	// handle joysticks events
	void onJoystickAxisMove(SDL_Event& event);
	void onJoystickButtonDown(SDL_Event& event);
	void onJoystickButtonUp(SDL_Event& event);
};
typedef InputHandler TheInputHandler;