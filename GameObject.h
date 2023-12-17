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
		m_pos.setX(0); 
		m_pos.setY(0);

		m_Collider.x = 0;
		m_Collider.y = 0;
		m_Collider.w = 0;
		m_Collider.h = 0;
	}

	// draw the object
	virtual void draw(SDL_Renderer* m_pRenderer) = 0;
	// get the type of the object
	virtual std::string type() = 0;
	// getters for common variable
	Vector2D& getPosition() { return m_pos; }
	SDL_Rect& getCollider() { return m_Collider; }

protected:
	// constructor with default initialisation list
	GameObject() : m_pos ( 0, 0 )
	{
		m_Collider.x = 0;
		m_Collider.y = 0;
		m_Collider.w = 0;
		m_Collider.h = 0;
	}
	// position
	Vector2D m_pos;
	// collision rectangle
	SDL_Rect m_Collider;
};