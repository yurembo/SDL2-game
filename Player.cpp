#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Player.h"
#include "InputHandler.h"
#include "Game.h"

Player::Player() : m_vel(0,0)
{
	m_pos.setX(SCREEN_WIDTH / 2);
	m_pos.setY(SCREEN_HEIGHT / 2);
	m_score = 0;

	moveCollider();
}

Player::~Player()
{
	m_pos.setX(0);
	m_pos.setY(0);

	m_vel.setX(0);
	m_vel.setY(0);

	m_Collider.h = 0;
	m_Collider.w = 0;
	m_Collider.x = 0;
	m_Collider.y = 0;

	m_score = 0;
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
	//m_pos -= m_vel;
	/*
	// Вычисляем относительную скорость
	Vector2D rv = m_vel;

	// Вычисляем относительную скорость относительно направления нормали
	float velAlongNormal = rv.dot(normal);

	// Не выполняем вычислений, если скорости разделены
	if (velAlongNormal > 0);
		return;

	// Вычисляем упругость
	//float e = min(A.restitution, B.restitution);

	// Вычисляем скаляр импульса силы
	float j = -1 * velAlongNormal;
	//j /= 1 / A.mass + 1 / B.mass;

	// Прикладываем импульс силы
	Vector2D impulse = j * normal;
	m_vel = -1 / impulse;
	*/
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
		m_pos.setX(m_pos.getX() + INC_STEP);
    }
	if (TheInputHandler::Instance()->getAxisX(0, 1) < 0)
	{
		m_pos.setX(m_pos.getX() - INC_STEP);
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) > 0)
	{
		m_pos.setY(m_pos.getY() + INC_STEP);
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) < 0)
	{
		m_pos.setY(m_pos.getY() - INC_STEP);
	}
}

void Player::setVelocity(const Vector2D vel)
{
	m_vel += vel;
}

bool Player::checkCollisionWithPolygon(SDL_Renderer* m_pRenderer, const std::vector<Sint16>& m_vertexX, const std::vector<Sint16>& m_vertexY)
{
	std::vector<Sint16>::size_type i = 0;
	int lastX = 0;
	int lastY = 0;
	//SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	int numv = m_vertexX.size() + 1;

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
			//SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
		}
		else
			if ((i + 1 == m_vertexX.size()))
			{
				x2 = m_vertexX.at(0);
				y2 = m_vertexY.at(0);
				//SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
			}
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

void Player::moveCollider()
{
	m_Collider.x = m_pos.getX() - PLAYER_RADIUS / 2;
	m_Collider.w = PLAYER_RADIUS;
	m_Collider.y = m_pos.getY() - PLAYER_RADIUS / 2;
	m_Collider.h = PLAYER_RADIUS;
}