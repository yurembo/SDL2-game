#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include "FieldOfView.h"

Player::Player() : m_vel(0.f,0.f), m_score(0), m_targetPos(0.f,0.f), m_inertia(false), m_GamePad(false)
{
	m_pos.setX(SCREEN_WIDTH / 2);
	m_pos.setY(SCREEN_HEIGHT / 2);

	moveCollider();
}

Player::~Player()
{
	m_pos.setX(0.f);
	m_pos.setY(0.f);

	m_vel.setX(0.f);
	m_vel.setY(0.f);

	m_Collider.h = 0;
	m_Collider.w = 0;
	m_Collider.x = 0;
	m_Collider.y = 0;

	m_score = 0;

	m_targetPos.setX(0.f);
	m_targetPos.setY(0.f);

	m_inertia = false;
	m_GamePad = false;
}

void Player::draw(SDL_Renderer* m_pRenderer)
{
	TheField::Instance()->draw(m_pRenderer);

	filledCircleRGBA(m_pRenderer, static_cast<Sint16>(m_pos.getX()), static_cast<Sint16>(m_pos.getY()), PLAYER_RADIUS, 255, 255, 0, 255);
}

void Player::update()
{
	if (m_inertia) 
		setInertia();
	
	if (m_GamePad)
		handleInput();

	m_pos += m_vel;
	moveCollider();

	TheField::Instance()->update(m_pos.getX(), m_pos.getY());
}

void Player::resolveCollision(const Polygon& poly)
{
	const float divider = 8;

 	Vector2D vecDiff = Vector2D::norm(m_vel);
 	float Dist = PLAYER_RADIUS; 
 	Dist = sqrt(Dist);
 	float ToPush = PLAYER_RADIUS - Dist;
 	vecDiff *= ToPush;
 	m_pos -= vecDiff;
	m_targetPos = m_pos - vecDiff;

	m_vel = -m_vel / divider;
	m_inertia = true;
}

inline std::string Player::type()
{
	return "Player";
}

// player is controlled by a gamepad
void Player::handleInput()
{
	Vector2D vel(0,0);
	if (TheInputHandler::Instance()->getAxisX(0, 1) > 0)
    {
		vel.setX(INC_STEP);
    }
	if (TheInputHandler::Instance()->getAxisX(0, 1) < 0)
	{
		vel.setX(-INC_STEP);
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) > 0)
	{
		vel.setY(INC_STEP);
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) < 0)
	{
		vel.setY(-INC_STEP);
	}
	m_vel = vel;
}

void Player::setVelocity(const Vector2D& vel)
{
	m_vel += vel;
}

bool Player::checkCollisionWithPolygon(SDL_Renderer* m_pRenderer, const std::vector<Sint16>& m_vertexX, const std::vector<Sint16>& m_vertexY) 
{
	std::vector<Sint16>::size_type i = 0;
	//SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	while (i < m_vertexX.size())
	{
		int x1 = m_vertexX.at(i);
		int y1 = m_vertexY.at(i);
		int x2 = 0;
		int y2 = 0;
		if (i + 1 < m_vertexX.size())
		{
			x2 = m_vertexX.at(i + 1);
			y2 = m_vertexY.at(i + 1);
		}
		else
			if (i + 1 == m_vertexX.size())
			{
				x2 = m_vertexX.at(0);
				y2 = m_vertexY.at(0);
			}
		//SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2); - to draw polygons' edges
		if (SDL_IntersectRectAndLine(&m_Collider, &x1, &y1, &x2, &y2))
		{
			return true;
		}
		i++;
	}
	return false;
}

bool Player::checkCollisionWithBonus(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	if (SDL_HasIntersection(&rect1, &rect2) == SDL_TRUE)
	{
		m_score++;
		return true;
	}
	return false;
}

void Player::setScore(const int score)
{
	m_score += score;
}

int Player::getScore() const
{
	return m_score;
}

void Player::setInertia()
{
	const float bound = 0.15f;

	if (m_vel.getX() < 0)
		m_vel.setX(m_vel.getX() + bound);
	if (m_vel.getX() > 0)
		m_vel.setX(m_vel.getX() - bound);
	if (m_vel.getY() > 0)
		m_vel.setY(m_vel.getY() - bound);
	if (m_vel.getY() < 0)
		m_vel.setY(m_vel.getY() + bound);

	if (round(m_vel.getX()) == 0.f && round(m_vel.getY()) == 0.f) 
	{
		m_inertia = false;
		m_vel.setX(0.f);
		m_vel.setY(0.f);
	}
 	else
 		m_inertia = true;
}

void Player::moveCollider()
{
	m_Collider.x = static_cast<int>(m_pos.getX() - PLAYER_RADIUS / 2);
	m_Collider.w = PLAYER_RADIUS;
	m_Collider.y = static_cast<int>(m_pos.getY() - PLAYER_RADIUS / 2);
	m_Collider.h = PLAYER_RADIUS;
}