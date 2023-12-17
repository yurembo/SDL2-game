#pragma once

#include <vector>
#include "GameObject.h"

class Bonus : public GameObject
{
public:
	Bonus(const int xpos, const int ypos);
	virtual ~Bonus() override;
	virtual void draw(SDL_Renderer* m_pRenderer) override;
	virtual std::string type() override;
	//SDL_Rect getCollider() const;
	
private:
	//hidden rectangle to find a place to create a bonus
	//SDL_Rect m_Collider;
};