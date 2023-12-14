#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <algorithm>
#include "Polygon.h"

Polygon::Polygon(const std::vector<int>& x_coords, const std::vector<int>& y_coords)
{
	for (auto iter = x_coords.begin(); iter != x_coords.end(); ++iter)
		m_vertexX.push_back(*iter);

	for (auto iter = y_coords.begin(); iter != y_coords.end(); ++iter)
		m_vertexY.push_back(*iter);

	//find maximum and minimum values of the vectors (std:C++17 is needed)
	const auto [minX, maxX] = std::minmax_element(x_coords.begin(), x_coords.end());
	const auto [minY, maxY] = std::minmax_element(y_coords.begin(), y_coords.end());

	m_Collider.x = *minX;
	m_Collider.y = *minY;
	m_Collider.w = *maxX - *minX;
	m_Collider.h = *maxY - *minY;
}

Polygon::~Polygon()
{
	m_vertexX.clear();
	m_vertexY.clear();

	m_Collider.x = 0;
	m_Collider.y = 0;
	m_Collider.w = 0;
	m_Collider.h = 0;
}

void Polygon::draw(SDL_Renderer* m_pRenderer)
{
	filledPolygonRGBA(m_pRenderer, m_vertexX.data(), m_vertexY.data(), m_vertexX.size(), 0, 0, 255, 255);
}

std::string Polygon::type()
{
	return "Polygon";
}

SDL_Rect Polygon::getCollider() const
{
	return m_Collider;
}

std::vector<Sint16> Polygon::getVertexX() const
{
	return m_vertexX;
}

std::vector<Sint16> Polygon::getVertexY() const
{
	return m_vertexY;
}
