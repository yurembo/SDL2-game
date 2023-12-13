#pragma once

#include <string>
#include <SDL.h>
#include "const.h"
#include "Vector2D.h"

class GameObject
{
public:
	// base class needs virtual destructor
	virtual ~GameObject() 
	{ 
		m_pos.m_x = 0; 
		m_pos.m_y = 0;
	}

	// draw the object
	virtual void draw(SDL_Renderer* m_pRenderer) = 0;
	// get the type of the object
	virtual std::string type() = 0;
	// getters for common variable
	Vector2D& getPosition() { return m_pos; }

protected:
	// constructor with default initialisation list
	GameObject() : m_pos(0, 0)
	{

	}
	// position
	Vector2D m_pos;
};