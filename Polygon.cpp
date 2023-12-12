#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Polygon.h"
/*
const std::vector< Sint16 > verts1 =
{
	{  400 },
	{  200 },
	{  600 },
};

const std::vector< Sint16 > verts2 =
{
	{  150 },
	{  450 },
	{  650 },
};
*/

Polygon::Polygon(const std::vector<int> x_coords, const std::vector<int> y_coords)
{
	for (auto iter = x_coords.begin(); iter != x_coords.end(); ++iter)
		m_vertexX.push_back(*iter);

	for (auto iter = y_coords.begin(); iter != y_coords.end(); ++iter)
		m_vertexY.push_back(*iter);
}

Polygon::~Polygon()
{
	m_vertexX.clear();
	m_vertexY.clear();
}

void Polygon::draw(SDL_Renderer* m_pRenderer)
{
	filledPolygonRGBA(m_pRenderer, m_vertexX.data(), m_vertexY.data(), m_vertexX.size(), 255, 0, 255, 255);
}

void Polygon::collision()
{

}

std::string Polygon::type()
{
	return "Polygon";
}
