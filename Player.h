#pragma once

#include "GameObject.h"
#include "Circle.h"
#include "Polygon.h"

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
	void resolveCollision();
	virtual std::string type() override; 
	// handle any input from the keyboard or joystick
	void handleInput();
	void setVelocity(const Vector2D vel);

	bool checkCollision(const std::vector<Sint16>& m_vertexX, const std::vector<Sint16>& m_vertexY);
	
private:
	// velocity
	Vector2D m_vel;
	//hidden rectangle to check a collision
	SDL_Rect m_Collider;
};