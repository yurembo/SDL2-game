#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Bonus.h"
#include "const.h"

Bonus::Bonus(const int xpos, const int ypos)
{
	m_pos.setX(xpos);
	m_pos.setY(ypos);

	m_Collider.x = xpos;
	m_Collider.y = ypos;
	m_Collider.w = BONUS_RECT_WIDTH;
	m_Collider.h = BONUS_RECT_WIDTH;
}

Bonus::~Bonus()
{
	m_Collider.x = 0;
	m_Collider.y = 0;
	m_Collider.w = 0;
	m_Collider.h = 0;
}

void Bonus::draw(SDL_Renderer* m_pRenderer)
{
	boxRGBA(m_pRenderer, m_pos.getX(), m_pos.getY(), m_pos.getX() + BONUS_RECT_WIDTH, m_pos.getY() + BONUS_RECT_WIDTH, 0, 255, 0, 255);
}

std::string Bonus::type()
{
	return "Bonus";
}

// SDL_Rect Bonus::getCollider() const
// {
// 	return m_Collider;
// }
