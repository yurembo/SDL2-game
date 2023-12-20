#include <iostream>
#include <SDL.h>
#include "InputHandler.h"
#include "Game.h"
#include "Player.h"

InputHandler* InputHandler::s_pInstance = 0;

InputHandler::InputHandler() : m_bJoysticksInitialised(false)
{

}

InputHandler::~InputHandler()
{
	// clear our arrays
	m_joystickValues.clear();
	m_joysticks.clear();
}

void InputHandler::initialiseJoysticks()
{
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
	{
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	}
	if (SDL_NumJoysticks() > 0)
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_Joystick* joy = SDL_JoystickOpen(i);
			if (joy)
			{
				m_joysticks.push_back(joy);
				m_joystickValues.push_back(std::make_pair(new Vector2D(0, 0), new Vector2D(0, 0)));
			}
			else
			{
				std::cout << SDL_GetError();
			}
		}
		SDL_JoystickEventState(SDL_ENABLE);
		m_bJoysticksInitialised = true;
		std::cout << "Initialised " << m_joysticks.size() << " joystick(s)\n";
	}
	else
	{
		m_bJoysticksInitialised = false;
	}
}

void InputHandler::clean()
{
	if (m_bJoysticksInitialised)
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(m_joysticks[i]);
		}
	}
}

void InputHandler::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			TheGame::Instance()->quit();
			break;
		case SDL_KEYDOWN:
			if (event.key.repeat == 0)
				onKeyDown(event);
			break;
		case SDL_KEYUP:
			if (event.key.repeat == 0)
				onKeyUp(event);
			break;
		case SDL_JOYAXISMOTION:
			onJoystickAxisMove(event);
			break;			
		default:
			break;
		}
	}
}

void InputHandler::onKeyDown(SDL_Event& e)
{
	if (e.key.keysym.sym == SDLK_ESCAPE)
	{
		TheGame::Instance()->quit();
	}

	// || TheGame::Instance()->getInertia())

 	if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) 
	{
 		Vector2D vel(0, -INC_STEP);
 		TheGame::Instance()->setPlayerVelocity(vel); 
 	}else
	if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s)
	{
		Vector2D vel(0, INC_STEP);
		TheGame::Instance()->setPlayerVelocity(vel);
	}
	if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a)
	{
		Vector2D vel(-INC_STEP, 0);
		TheGame::Instance()->setPlayerVelocity(vel);
	}else
	if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d)
	{
		Vector2D vel(INC_STEP, 0);
		TheGame::Instance()->setPlayerVelocity(vel);
	}
}

void InputHandler::onKeyUp(SDL_Event& e)
{
	//if (TheGame::Instance()->getCollisionDetect())// || TheGame::Instance()->getInertia())
		//return;

	if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w)
	{
		Vector2D vel(0, INC_STEP);
		TheGame::Instance()->setPlayerInertia();// setPlayerVelocity(vel);
	}
	if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s)
	{
		Vector2D vel(0, -INC_STEP);
		TheGame::Instance()->setPlayerInertia();
	}
	if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a)
	{
		Vector2D vel(INC_STEP, 0);
		TheGame::Instance()->setPlayerInertia();
	}
	if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d)
	{
		Vector2D vel(-INC_STEP, 0);
		TheGame::Instance()->setPlayerInertia();
	}
}

void InputHandler::onJoystickAxisMove(SDL_Event& event)
{
	int whichOne = event.jaxis.which;

	// left stick move left or right
	if (event.jaxis.axis == 0)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[whichOne].first->setX(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[whichOne].first->setX(-1);
		}
		else
		{
			m_joystickValues[whichOne].first->setX(0);
		}
	}

	// left stick move up or down
	if (event.jaxis.axis == 1)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[whichOne].first->setY(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[whichOne].first->setY(-1);
		}
		else
		{
			m_joystickValues[whichOne].first->setY(0);
		}
	}

	// right stick move left or right
	if (event.jaxis.axis == 3)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[whichOne].second->setX(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[whichOne].second->setX(-1);
		}
		else
		{
			m_joystickValues[whichOne].second->setX(0);
		}
	}

	// right stick move up or down
	if (event.jaxis.axis == 4)
	{
		if (event.jaxis.value > m_joystickDeadZone)
		{
			m_joystickValues[whichOne].second->setY(1);
		}
		else if (event.jaxis.value < -m_joystickDeadZone)
		{
			m_joystickValues[whichOne].second->setY(-1);
		}
		else
		{
			m_joystickValues[whichOne].second->setY(0);
		}
	}
}

float InputHandler::getAxisX(int joy, int stick) const
{
	if (m_joystickValues.size() > 0)
	{
		if (stick == 1)
		{
			return m_joystickValues[joy].first->getX();
		}
		else if (stick == 2)
		{
			return m_joystickValues[joy].second->getX();
		}
	}
	return 0;
}

float InputHandler::getAxisY(int joy, int stick) const
{
	if (m_joystickValues.size() > 0)
	{
		if (stick == 1)
		{
			return m_joystickValues[joy].first->getY();
		}
		else if (stick == 2)
		{
			return m_joystickValues[joy].second->getY();
		}
	}
	return 0;
}