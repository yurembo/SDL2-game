#pragma once

#include "GameObject.h"
#include "Circle.h"

class Player : public GameObject
{
public:
	// radius of the circle
	static const int PLAYER_RADIUS = 30;

	Player();
	virtual ~Player();

	void moveCollider();
	virtual void draw(SDL_Renderer* m_pRenderer) override;
	void update();
	void collision();
	virtual std::string type() override; 
	// handle any input from the keyboard or joystick
	void handleInput();
	void setVelocity(const Vector2D vel);

	//Circle/Box collision detector
	bool checkCollision(SDL_Rect& b);
	__int32 distanceSquared(int x1, int y1, int x2, int y2);
	
private:
	// velocity
	Vector2D m_vel;
	//hidden object to check a collision
	Circle m_Collider;
};