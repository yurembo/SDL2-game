#pragma once

#include "GameObject.h"

class Bonus : public GameObject
{
public:
	Bonus() {};
	Bonus(const float xpos, const float ypos);
	virtual ~Bonus() override;
	virtual void draw(SDL_Renderer* m_pRenderer) override;
	virtual std::string type() override;
};