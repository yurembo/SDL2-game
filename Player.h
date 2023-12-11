#pragma once

#include "GameObject.h"

class Player : public GameObject
{
public:

	Player();
	virtual ~Player() {}

	virtual void draw(SDL_Renderer* renderer) override;
	virtual void update();
	virtual void clean();

	virtual void collision();

	virtual std::string type() { return "Player"; }

	// handle any input from the keyboard or joystick
	void handleInput();

private:

	
};