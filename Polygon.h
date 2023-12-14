#pragma once

#include <vector>
#include "GameObject.h"

class Polygon : public GameObject
{
public:
	Polygon(const std::vector<int>& x_coords, const std::vector<int>& y_coords);
	virtual ~Polygon() override;
	virtual void draw(SDL_Renderer* m_pRenderer) override;
	virtual std::string type() override;
	SDL_Rect getCollider() const;
	std::vector<Sint16> getVertexX() const;
	std::vector<Sint16> getVertexY() const;
	
private:
	std::vector<Sint16>m_vertexX;
	std::vector<Sint16>m_vertexY;
	//hidden rectangle to find a place to create a bonus
	SDL_Rect m_Collider;
};

