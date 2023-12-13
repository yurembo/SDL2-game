#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Player.h"
#include "InputHandler.h"
#include "Game.h"

Player::Player() : m_vel(0,0)
{
	m_pos.m_x = SCREEN_WIDTH / 2;
	m_pos.m_y = SCREEN_HEIGHT / 2;

	m_Collider.rad = PLAYER_RADIUS / 3;
	moveCollider();
}

Player::~Player()
{
	m_pos.m_x = 0;
	m_pos.m_y = 0;
	m_vel.m_x = 0;
	m_vel.m_y = 0;
	m_Collider.pos.m_x = 0;
	m_Collider.pos.m_x = 0;
	m_Collider.rad = 0;
}

void Player::draw(SDL_Renderer* m_pRenderer)
{
	filledCircleRGBA(m_pRenderer, m_pos.getX(), m_pos.getY(), PLAYER_RADIUS, 255, 255, 0, 255);
}

void Player::update()
{
	handleInput();
	m_pos += m_vel;
	moveCollider();
	
}

void Player::collision()
{

}

inline std::string Player::type()
{
	return "Player";
}

// player controlled by a gamepad
void Player::handleInput()
{
	if (TheInputHandler::Instance()->getAxisX(0, 1) > 0)
    {
		m_pos.m_x += INC_STEP;
    }
	if (TheInputHandler::Instance()->getAxisX(0, 1) < 0)
	{
		m_pos.m_x -= INC_STEP;
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) > 0)
	{
		m_pos.m_y += INC_STEP;
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) < 0)
	{
		m_pos.m_y -= INC_STEP;
	}
}

void Player::setVelocity(const Vector2D vel)
{
	m_vel += vel;
}

bool Player::checkCollision(SDL_Rect& b)
{
	Circle& a = m_Collider;
	//Closest point on collision box
	int cX, cY;
	//Find closest x offset
	if (a.pos.getX() < b.x)
	{
		cX = b.x;
	}
	else if (a.pos.getX() > b.x + b.w)
	{
		cX = b.x + b.w;
	}
	else
	{
		cX = a.pos.getX();
	}
	//Find closest y offset
	if (a.pos.getY() < b.y)
	{
		cY = b.y;
	}
	else if (a.pos.getY() > b.y + b.h)
	{
		cY = b.y + b.h;
	}
	else
	{
		cY = a.pos.getY();
	}
	//If the closest point is inside the circle
	if (distanceSquared(a.pos.getX(), a.pos.getY(), cX, cY) < a.rad * a.rad)
	{
		//This box and the circle have collided
		return true;
	}
	//If the shapes have not collided
	return false;
}

//__int32 to avoid a compiler overflow warning
__int32 Player::distanceSquared(int x1, int y1, int x2, int y2)
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX * deltaX + deltaY * deltaY;
}

void Player::moveCollider()
{
	m_Collider.pos.setX(m_pos.getX());
	m_Collider.pos.setY(m_pos.getY());
}