#pragma once

#include "GameObject.h"
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
	void resolveCollision(const Polygon& poly);
	virtual std::string type() override; 
	// handle any input from the keyboard or joystick
	void handleInput();
	void setVelocity(const Vector2D& vel);
	const Vector2D& getVelocity() { return m_vel; }

	bool checkCollisionWithPolygon(SDL_Renderer* m_pRenderer, const std::vector<Sint16>& m_vertexX, const std::vector<Sint16>& m_vertexY);
	bool checkCollisionWithBonus(const SDL_Rect& rect1, const SDL_Rect& rect2);

	void setScore(const int score);
	int getScore() const;

	void setInertia();
	bool getInertia() { return m_inertia; }

	void pickUpGamePad() { m_GamePad = true; }
private:
	int m_score;
	// velocity
	Vector2D m_vel;
	// targeting position after collision
	Vector2D m_targetPos;
	//
	bool m_inertia;
	bool m_GamePad;
};