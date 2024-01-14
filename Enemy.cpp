#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "const.h"
#include "Enemy.h"

Enemy::Enemy(const float xpos, const float ypos)
{
	m_pos.setX(xpos);
	m_pos.setY(ypos);

	moveCollider();
}

Enemy::~Enemy()
{
	m_vel.setX(0);
	m_vel.setY(0);

	SDL_RemoveTimer(m_timerID);
}

void Enemy::draw(SDL_Renderer* m_pRenderer)
{
	boxRGBA(m_pRenderer, (int)m_pos.getX() - BONUS_RECT_WIDTH / 2, (int)m_pos.getY() - BONUS_RECT_WIDTH / 2,
	(int)m_pos.getX() + BONUS_RECT_WIDTH / 2, (int)m_pos.getY() + BONUS_RECT_WIDTH / 2, 255, 0, 0, 255);
}

std::string Enemy::type()
{
	return "Enemy";
}

void Enemy::moveToPlayer(Vector2D& playerPos)
{
	if (m_inertia) 
		return;

	const int speed = 1;

 	if (playerPos.getX() < m_pos.getX())
 		m_vel.setX(-speed); 
 	if (playerPos.getX() > m_pos.getX())
 		m_vel.setX(speed); 
 	if (playerPos.getY() < m_pos.getY())
 		m_vel.setY(-speed);
 	if (playerPos.getY() > m_pos.getY())
 		m_vel.setY(speed);

	m_pos += m_vel;
	moveCollider();
}

void Enemy::moveCollider()
{
	m_Collider.x = (int)m_pos.getX() - BONUS_RECT_WIDTH / 2;
	m_Collider.y = (int)m_pos.getY() - BONUS_RECT_WIDTH / 2;
	m_Collider.w = BONUS_RECT_WIDTH;
	m_Collider.h = BONUS_RECT_WIDTH;
}

Uint32 Enemy::callback(Uint32 interval, void* object)
{
	((Enemy*)object)->setInertiaOff();
	return TIMER_INTERVAL;
}

void Enemy::setInertiaOff()
{
	m_inertia = false;
	bool boo = SDL_RemoveTimer(m_timerID);
	SDL_RemoveTimer(m_timerID);
	m_timerID = 0;
	std::cout << "OFF" << std::endl;
}

void Enemy::resolveCollision(Vector2D& vel)
{
	const int multiplier = 16;

	if (m_vel.getX() == 0 && m_vel.getY() == 0 && (vel.getX() != 0 || vel.getY() != 0)) 
	{
		m_pos = m_pos + vel * 2;
		m_vel = vel;

		m_inertia = true;
		m_timerID = SDL_AddTimer(TIMER_INTERVAL, &Enemy::callback, this);
	} 
	else
	if (m_vel.getX() != 0 || m_vel.getY() != 0)
	{
		Vector2D vecDiff = Vector2D::normVec(m_vel);
		float Dist = PLAYER_RADIUS;
		Dist = sqrt(Dist);
		float ToPush = PLAYER_RADIUS - Dist;
		vecDiff *= ToPush;
		m_pos -= vecDiff;
		m_vel = -m_vel / multiplier;

		m_inertia = true;
		m_timerID = SDL_AddTimer(TIMER_INTERVAL, &Enemy::callback, this);
	}
	moveCollider();
}
