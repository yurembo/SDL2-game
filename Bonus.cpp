#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Bonus.h"
#include "const.h"

Bonus::Bonus(const float xpos, const float ypos)
{
	m_pos.setX(xpos);
	m_pos.setY(ypos);

	m_Collider.x = (int)xpos - BONUS_RECT_WIDTH / 2;
	m_Collider.y = (int)ypos - BONUS_RECT_WIDTH / 2;
	m_Collider.w = BONUS_RECT_WIDTH;
	m_Collider.h = BONUS_RECT_WIDTH;
}

Bonus::~Bonus()
{

}

void Bonus::draw(SDL_Renderer* m_pRenderer)
{
	boxRGBA(m_pRenderer, (int)m_pos.getX() - BONUS_RECT_WIDTH / 2, (int)m_pos.getY() - BONUS_RECT_WIDTH / 2,
	(int)m_pos.getX() + BONUS_RECT_WIDTH / 2, (int)m_pos.getY() + BONUS_RECT_WIDTH / 2, 0, 255, 0, 255);
}

std::string Bonus::type()
{
	return "Bonus";
}
