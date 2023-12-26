#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Player.h"
#include "InputHandler.h"
#include "Game.h"
#include <algorithm>

Player::Player() : m_vel(0.f,0.f), m_score(0), m_targetPos(0.f,0.f), m_inertia(false), m_GamePad(false)
{
	m_pos.setX(SCREEN_WIDTH / 2);
	m_pos.setY(SCREEN_HEIGHT / 2);

	moveCollider();
}

Player::~Player()
{
	m_pos.setX(0.f);
	m_pos.setY(0.f);

	m_vel.setX(0.f);
	m_vel.setY(0.f);

	m_Collider.h = 0;
	m_Collider.w = 0;
	m_Collider.x = 0;
	m_Collider.y = 0;

	m_score = 0;

	m_targetPos.setX(0.f);
	m_targetPos.setY(0.f);

	m_inertia = false;
	m_GamePad = false;
}

void Player::draw(SDL_Renderer* m_pRenderer)
{
	filledCircleRGBA(m_pRenderer, static_cast<Sint16>(m_pos.getX()), static_cast<Sint16>(m_pos.getY()), PLAYER_RADIUS, 255, 255, 0, 255);
}

void Player::update()
{
	if (m_inertia) 
		setInertia();
	
	if (m_GamePad)
		handleInput();

	m_pos += m_vel;
	moveCollider();
}

void Player::resolveCollision(const Polygon& poly)
{
	const float divider = 8;

 	Vector2D vecDiff = Vector2D::normVec(m_vel);
 	float Dist = PLAYER_RADIUS; 
 	Dist = sqrt(Dist);
 	float ToPush = PLAYER_RADIUS - Dist;
 	vecDiff *= ToPush;
 	m_pos -= vecDiff;
	m_targetPos = m_pos - vecDiff;

	m_vel = -m_vel / divider;
	m_inertia = true;
}

inline std::string Player::type()
{
	return "Player";
}

// player is controlled by a gamepad
void Player::handleInput()
{
	Vector2D vel(0,0);
	if (TheInputHandler::Instance()->getAxisX(0, 1) > 0)
    {
		vel.setX(INC_STEP);
    }
	if (TheInputHandler::Instance()->getAxisX(0, 1) < 0)
	{
		vel.setX(-INC_STEP);
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) > 0)
	{
		vel.setY(INC_STEP);
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) < 0)
	{
		vel.setY(-INC_STEP);
	}
	m_vel = vel;
}

void Player::setVelocity(const Vector2D& vel)
{
	m_vel += vel;
}

bool Player::checkCollisionWithPolygon(SDL_Renderer* m_pRenderer, const std::vector<Sint16>& vertexX, const std::vector<Sint16>& vertexY) 
{
	std::vector<Sint16>::size_type i = 0;
	//SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

	while (i < vertexX.size())
	{
		std::vector<int> vec = getSegment(i, vertexX, vertexY);
		if (!vec.empty())
		{
			//SDL_RenderDrawLine(m_pRenderer, vec[0], vec[1], vec[2], vec[3]); // - to draw polygons' edges
			if (SDL_IntersectRectAndLine(&m_Collider, &vec[0], &vec[1], &vec[2], &vec[3]))
			{
				return true;
			}
		}
		i++;
	}
	return false;
}

std::vector<int> Player::getSegment(const unsigned int index, const std::vector<Sint16>& vertexX, const std::vector<Sint16>& vertexY) const
{
	if (!vertexX.empty() && !vertexY.empty())
	{
		int x1 = vertexX.at(index);
		int y1 = vertexY.at(index);
		int x2 = 0;
		int y2 = 0;
		if (index + 1 < vertexX.size())
		{
			x2 = vertexX.at(index + 1);
			y2 = vertexY.at(index + 1);
		}
		else
			if (index + 1 == vertexX.size())
			{
				x2 = vertexX.at(0);
				y2 = vertexY.at(0);
			}
		std::vector<int> resVec { x1, y1, x2, y2 };
		return resVec;
	}
	std::vector<int> resVec;
	resVec.clear();

	return resVec;
}

bool Player::getMinDistance(std::pair<Vector2D, float> param1, std::pair<Vector2D, float> param2)
{
	return param1.second < param2.second;
}

void Player::ShootRays(SDL_Renderer* m_pRenderer, const std::vector<Sint16>& vertexX, const std::vector<Sint16>& vertexY, std::vector<Vector2D>& out_intersectDots)
{
	const float pi2 = static_cast<float>(M_PI) * 2;
	const int raysCount = 6;
	
	//for (float angle = 0; angle < pi2; angle += pi2 / raysCount)
	for (int angle = 0; angle < 360; angle += 60)
	{
		float dx = m_pos.getX() + RAY_LENGTH * std::cosf(angle);
		float dy = m_pos.getY() + RAY_LENGTH * std::sinf(angle);

		Vector2D v1 { m_pos.getX(), m_pos.getY() };
		Vector2D v2 { dx, dy };
		std::pair<Vector2D, Vector2D> ray { v1, v2 };
		std::vector<std::pair<Vector2D, float>>vpoints;

		for (std::vector<Sint16>::size_type i = 0; i < vertexX.size(); ++i)
		{
			std::vector<int> segment = getSegment(i, vertexX, vertexY);
			if (!segment.empty())
			{
				Vector2D v3 { float(segment[0]), float(segment[1]) };
				Vector2D v4 { float(segment[2]), float(segment[3]) };
				std::pair<Vector2D, Vector2D> seg { v3, v4 };
				std::pair<Vector2D, float>intersect = getIntersect(ray, seg);
				Vector2D point = intersect.first;
				float param2 = intersect.second;
				
  				if (point.getX() == 0 && point.getY() == 0 && param2 == 0) // no intersect
 				{
  					continue;
 				}

				float dist = Vector2D::getDistance(m_pos, point);
				std::pair<Vector2D, float>p{point, dist};
				vpoints.push_back(p);
			}	
		}
		auto dot = getMinElem(vpoints);
		if (dot.getX() != 0.f && dot.getY() != 0.f)
			out_intersectDots.push_back(dot);
	}
}

Vector2D Player::getMinElem(const std::vector<std::pair<Vector2D, float>>& vpoints) const
{
	if (!vpoints.empty())
	{
		auto p = std::min_element(vpoints.begin(), vpoints.end());
		auto elem = p->first;
		return Vector2D { elem.getX(), elem.getY() };
	}
	return Vector2D { 0, 0 };
}
 
std::pair<Vector2D, float> Player::getIntersect(const std::pair<Vector2D, Vector2D>& ray, const std::pair<Vector2D, Vector2D>& segment)
{
	Vector2D v {0,0};
	std::pair<Vector2D, float> pair { v, 0.f };
	
 	// RAY in parametric: Point + Delta*T1
	float r_px = Vector2D(ray.first).getX();
	float r_py = Vector2D(ray.first).getY();
	float r_dx = Vector2D(ray.second).getX() - r_px;
	float r_dy = Vector2D(ray.second).getY() - r_py;
 
 	// SEGMENT in parametric: Point + Delta*T2
	float s_px = Vector2D(segment.first).getX();
	float s_py = Vector2D(segment.first).getY();
	float s_dx = Vector2D(segment.second).getX() - s_px;
	float s_dy = Vector2D(segment.second).getY() - s_py;
 
 	// Are they parallel? If so, no intersect
	float r_mag = sqrt(r_dx * r_dx + r_dy * r_dy);
	float s_mag = sqrt(s_dx * s_dx + s_dy * s_dy);
	if (r_dx / r_mag == s_dx / s_mag && r_dy / r_mag == s_dy / s_mag)
	{
		return pair;
	}
 
 	// SOLVE FOR T1 & T2
	float T2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px)) / (s_dx * r_dy - s_dy * r_dx);
	float T1 = (s_px + s_dx * T2 - r_px) / r_dx;
 
 	// Must be within parametic whatevers for RAY/SEGMENT
 	if (T1 < 0) 
	{
		return pair;
	}

	if (T2 < 0 || T2 > 1)
	{
		return pair;	
	}
 
 	// Return the POINT OF INTERSECTION
 	float x = r_px + r_dx * T1;
	float y = r_py + r_dy * T1;
	v.setX(x);
	v.setY(y);
	pair.first = v;
	pair.second = T1;

	return pair;
 }

bool Player::checkCollisionWithBonus(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	if (SDL_HasIntersection(&rect1, &rect2) == SDL_TRUE)
	{
		m_score++;
		return true;
	}
	return false;
}

void Player::setScore(const int score)
{
	m_score += score;
}

int Player::getScore() const
{
	return m_score;
}

void Player::setInertia()
{
	const float bound = 0.15f;

	if (m_vel.getX() < 0)
		m_vel.setX(m_vel.getX() + bound);
	if (m_vel.getX() > 0)
		m_vel.setX(m_vel.getX() - bound);
	if (m_vel.getY() > 0)
		m_vel.setY(m_vel.getY() - bound);
	if (m_vel.getY() < 0)
		m_vel.setY(m_vel.getY() + bound);

	if (round(m_vel.getX()) == 0.f && round(m_vel.getY()) == 0.f) 
	{
		m_inertia = false;
		m_vel.setX(0.f);
		m_vel.setY(0.f);
	}
 	else
 		m_inertia = true;
}

void Player::moveCollider()
{
	m_Collider.x = static_cast<int>(m_pos.getX() - PLAYER_RADIUS / 2);
	m_Collider.w = PLAYER_RADIUS;
	m_Collider.y = static_cast<int>(m_pos.getY() - PLAYER_RADIUS / 2);
	m_Collider.h = PLAYER_RADIUS;
}