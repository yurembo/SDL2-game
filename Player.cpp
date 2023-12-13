#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include "Player.h"
#include "InputHandler.h"
#include "Game.h"

Player::Player() : m_vel(0,0)
{
	m_pos.m_x = SCREEN_WIDTH / 2 - PLAYER_RADIUS;
	m_pos.m_y = SCREEN_HEIGHT / 2 - PLAYER_RADIUS;
}

Player::~Player()
{
	m_pos.m_x = 0;
	m_pos.m_y = 0;
	m_vel.m_x = 0;
	m_vel.m_y = 0;
}

void Player::draw(SDL_Renderer* m_pRenderer)
{
	filledCircleRGBA(m_pRenderer, m_pos.getX(), m_pos.getY(), PLAYER_RADIUS, 255, 255, 0, 255);
}

void Player::update()
{
	handleInput();

	m_pos += m_vel;
}

void Player::collision()
{

}

inline std::string Player::type()
{
	return "Player";
}

void Player::handleInput()
{
/*
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
	else
	
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_DOWN) || TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
	{
		if (m_pos.m_y < SCREEN_HEIGHT - BORDER_WIDTH - PLAYER_RADIUS)
		{
			//m_pos.m_y += INC_STEP;
			m_vel.m_y += INC_STEP;
		}
		else m_pos.m_y = SCREEN_HEIGHT - BORDER_WIDTH - PLAYER_RADIUS;
	}
	if (TheInputHandler::Instance()->isKeyUp(SDL_SCANCODE_DOWN) || TheInputHandler::Instance()->isKeyUp(SDL_SCANCODE_S))
	{
		if (m_pos.m_y < SCREEN_HEIGHT - BORDER_WIDTH - PLAYER_RADIUS)
		{
			//m_pos.m_y += INC_STEP;
			m_vel.m_y -= INC_STEP;
		}
		else m_pos.m_y = SCREEN_HEIGHT - BORDER_WIDTH - PLAYER_RADIUS;
	}
	
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_UP) || TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
	{
		if (m_pos.m_y > BORDER_WIDTH + PLAYER_RADIUS)
		{
			//m_pos.m_y -= INC_STEP;
		}
		else m_pos.m_y = BORDER_WIDTH + PLAYER_RADIUS;
	}
	
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RIGHT) || TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
	{
		if (m_pos.m_x < SCREEN_WIDTH - BORDER_WIDTH - PLAYER_RADIUS)
		{
			//m_pos.m_x += INC_STEP;
		}
		else m_pos.m_x = SCREEN_WIDTH - BORDER_WIDTH - PLAYER_RADIUS;
	}
	
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_LEFT) || TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
	{
		if (m_pos.m_x > BORDER_WIDTH + PLAYER_RADIUS)
		{
			m_pos.m_x -= INC_STEP;
		} 
		else m_pos.m_x = BORDER_WIDTH + PLAYER_RADIUS;
	}
	*/
	if (TheInputHandler::Instance()->getAxisX(0, 1) > 0)
    {
		m_pos.m_x += INC_STEP;
    }
	if (TheInputHandler::Instance()->getAxisX(0, 1) < 0)
	{
		m_pos.m_x -= INC_STEP;
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) > 0)
	{
		m_pos.m_y += INC_STEP;
	}
	if (TheInputHandler::Instance()->getAxisY(0, 1) < 0)
	{
		m_pos.m_y -= INC_STEP;
	}
}

void Player::setVelocity(const Vector2D vel)
{
	m_vel += vel;
}
