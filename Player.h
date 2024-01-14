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

	virtual void moveCollider();
	virtual void draw(SDL_Renderer* m_pRenderer) override;
	virtual void update();
	virtual void resolveCollision(Vector2D& vel);
	virtual std::string type() override; 
	// handle any input from the keyboard or joystick
	void handleInput();
	void setVelocity(const Vector2D& vel);
	Vector2D& getVelocity() { return m_vel; }

	bool checkCollisionWithPolygon(SDL_Renderer* m_pRenderer, const std::vector<Sint16>& vertexX, const std::vector<Sint16>& vertexY);
	bool checkCollisionWithBonus(const SDL_Rect& rect1, const SDL_Rect& rect2);
	bool checkCollisionWithEnemy(const SDL_Rect& rect1, const SDL_Rect& rect2);
	std::vector<int> getSegment(const unsigned int index, const std::vector<Sint16>& vertexX, const std::vector<Sint16>& vertexY) const;
	void ShootRays(SDL_Renderer* m_pRenderer, const std::vector<GameObject*> gameObjs, std::vector<Vector2D>& out_intersectDots);
	Vector2D getMinElem(const std::vector<std::pair<Vector2D, float>>& vpoints) const;
	Vector2D getIntersection(const std::pair<Vector2D, Vector2D>& ray, const std::pair<Vector2D, Vector2D>& segment);

	void setScore(const int score);
	int getScore() const;

	void setInertia();
	bool getInertia() { return m_inertia; }

	void pickUpGamePad() { m_GamePad = true; }

protected:
	int m_score;
	// velocity
	Vector2D m_vel;
	//
	bool m_inertia;
	bool m_GamePad;
};