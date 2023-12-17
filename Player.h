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

	bool checkCollisionWithPolygon(SDL_Renderer* m_pRenderer, const std::vector<Sint16>& m_vertexX, const std::vector<Sint16>& m_vertexY);
	bool checkCollisionWithBonus(const SDL_Rect& rect1, const SDL_Rect& rect2);

	void setScore(const int score);
	int getScore() const;
	
private:
	int m_score;
	// velocity
	Vector2D m_vel;
};