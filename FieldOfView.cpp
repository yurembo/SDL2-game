#include <SDL2_gfxPrimitives.h>
#include "FieldOfView.h"

FieldOfView* FieldOfView::s_pInstance = 0;

void FieldOfView::update(const float xpos, const float ypos)
{
	m_pos.setX(xpos);
	m_pos.setY(ypos);
}

FieldOfView::FieldOfView()
{
	
}

FieldOfView::~FieldOfView()
{

}

void FieldOfView::draw(SDL_Renderer* m_pRenderer)
{
	ShootTheRay(m_pRenderer);
}

std::string FieldOfView::type()
{
	return "FieldOfView";
}

void FieldOfView::ShootTheRay(SDL_Renderer* m_pRenderer)
{
	const float pi2 = M_PI * 2;
	const float dist = 500;
	const int countOfRays = 50;

	for (float angle = 0; angle < pi2; angle += pi2 / countOfRays)
	{
		float dx = m_pos.getX() + dist * std::cosf(angle);
		float dy = m_pos.getY() + dist * std::sinf(angle);

		lineRGBA(m_pRenderer, m_pos.getX(), m_pos.getY(), dx, dy, 255, 0, 0, 155);
		//rectangleRGBA(m_pRenderer, m_pos.getX(), m_pos.getY(), dx, dy, 255, 0, 0, 155);
	}
}
