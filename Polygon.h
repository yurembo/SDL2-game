#pragma once

#include <vector>
#include "GameObject.h"

class Polygon : public GameObject
{
public:
	Polygon(const std::vector<int> x_coords, const std::vector<int> y_coords);
	virtual ~Polygon() override;

	virtual void draw(SDL_Renderer* m_pRenderer) override;
	
	virtual void collision() override;

	virtual std::string type() override;

private:
	std::vector<Sint16>m_vertexX;
	std::vector<Sint16>m_vertexY;
};

