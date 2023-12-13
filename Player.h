#pragma once

#include "GameObject.h"

class Player : public GameObject
{
public:

	Player();
	virtual ~Player();

	virtual void draw(SDL_Renderer* m_pRenderer) override;
	void update();

	virtual void collision() override;

	virtual std::string type() override; 

	// handle any input from the keyboard or joystick
	void handleInput();

	void setVelocity(const Vector2D vel);

private:
	// velocity
	Vector2D m_vel;
	
};