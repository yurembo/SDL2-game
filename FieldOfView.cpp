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
	//ShootTheRay(m_pRenderer);
}

std::string FieldOfView::type()
{
	return "FieldOfView";
}

