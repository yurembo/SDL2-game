#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Player.h"
#include "InputHandler.h"
#include "Game.h"

Player::Player() : m_vel(0,0)
{
	m_pos.m_x = SCREEN_WIDTH / 2;
	m_pos.m_y = SCREEN_HEIGHT / 2;

	moveCollider();
}

Player::~Player()
{
	m_pos.m_x = 0;
	m_pos.m_y = 0;

	m_vel.m_x = 0;
	m_vel.m_y = 0;

	m_Collider.h = 0;
	m_Collider.w = 0;
	m_Collider.x = 0;
	m_Collider.y = 0;
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

void Player::resolveCollision()
{
	m_pos -= m_vel;
}

inline std::string Player::type()
{
	return "Player";
}

// player is controlled by a gamepad
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

bool Player::checkCollision(const std::vector<Sint16>& m_vertexX, const std::vector<Sint16>& m_vertexY)
{
	std::vector<Sint16>::size_type i = 0;

	int lastX = 0;
	int lastY = 0;
	bool even = false;

	if (m_vertexX.size() % 2 == 0) // even number of elements
	{
		even = true;
		lastX = m_vertexX[0];// remember coordinates of the first vertex
		lastY = m_vertexY[0];// to create the last vertex of the last line
	}
	
	while (i < m_vertexX.size())
	{
		int x1 = m_vertexX.at(i);
		int y1 = m_vertexY.at(i);
		int x2 = 0;
		int y2 = 0;
		if (i + 1 < m_vertexX.size())
		{
			x2 = m_vertexX.at(i+1);
			y2 = m_vertexY.at(i+1);
		} 
		else
		if ((i + 1 == m_vertexX.size()) && (even))
		{
			x2 = lastX;
			y2 = lastY;
		}
		if (SDL_IntersectRectAndLine(&m_Collider, &x1, &y1, &x2, &y2))
		{
 			return true;
		}
		i++;
	}
	return false;
}

void Player::moveCollider()
{
	m_Collider.x = m_pos.getX() - PLAYER_RADIUS / 2;
	m_Collider.w = PLAYER_RADIUS;
	m_Collider.y = m_pos.getY() - PLAYER_RADIUS / 2;
	m_Collider.h = PLAYER_RADIUS;
}