#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Bonus.h"
#include "const.h"

Bonus::Bonus(const float xpos, const float ypos)
{
	m_pos.setX(xpos);
	m_pos.setY(ypos);

	m_Collider.x = (int)xpos;
	m_Collider.y = (int)ypos;
	m_Collider.w = BONUS_RECT_WIDTH;
	m_Collider.h = BONUS_RECT_WIDTH;
}

Bonus::~Bonus()
{

}

void Bonus::draw(SDL_Renderer* m_pRenderer)
{
	boxRGBA(m_pRenderer, static_cast<int>((int)m_pos.getX()), (int)m_pos.getY(), (int)m_pos.getX() + BONUS_RECT_WIDTH, (int)m_pos.getY() + BONUS_RECT_WIDTH, 0, 255, 0, 255);
}

std::string Bonus::type()
{
	return "Bonus";
}
