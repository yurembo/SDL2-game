#pragma once

#include "Player.h"

class Enemy : public Player
{
public:
	Enemy(const float xpos, const float ypos);
	virtual ~Enemy() override;
	virtual void draw(SDL_Renderer* m_pRenderer) override;
	virtual std::string type() override;
	void moveToPlayer(Vector2D& playerPos);
	virtual void moveCollider() override;
	virtual void resolveCollision(Vector2D& vel) override;

private:
	static const int TIMER_INTERVAL = 750;
	SDL_TimerID m_timerID;
	static Uint32 callback(Uint32 interval, void* object);
	void setInertiaOff();
};