#pragma once

#include "GameObject.h"

class FieldOfView : public GameObject
{
public:
	static FieldOfView* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new FieldOfView();
			return s_pInstance;
		}

		return s_pInstance;
	}
	virtual ~FieldOfView() override;
	virtual void draw(SDL_Renderer* m_pRenderer) override;
	virtual std::string type() override;

	static FieldOfView* s_pInstance;

	void update(const float xpos, const float ypos);
private:
	FieldOfView();
	FieldOfView(const FieldOfView&);
	FieldOfView& operator=(const FieldOfView&);

	void ShootTheRay(SDL_Renderer* m_pRenderer);

};
typedef FieldOfView TheField;