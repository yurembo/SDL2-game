#pragma once

#include <string>
#include <SDL.h>
#include "const.h"
#include "Vector2D.h"

class GameObject
{
public:

	// base class needs virtual destructor
	virtual ~GameObject() {}

	// draw the object
	virtual void draw(SDL_Renderer* m_pRenderer) = 0;

	// object has collided, handle accordingly
	virtual void collision() = 0;

	// get the type of the object
	virtual std::string type() = 0;

	// getters for common variables
	Vector2D& getPosition() { return m_pos; }

protected:

	// constructor with default initialisation list
	GameObject() : m_pos(0, 0)
	{

	}

	Vector2D m_pos;
};