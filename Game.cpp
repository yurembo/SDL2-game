#include <iostream>
#include <vector>
#include <SDL2_gfxPrimitives.h>
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"

Game* Game::s_pInstance = 0;

Game::Game() : m_pWindow(0), m_pRenderer(0), m_bRunning(false)
{

}

Game::~Game()
{
	m_pWindow = 0;
	m_pRenderer = 0;
}

bool Game::init(const char* title, int xpos, int ypos, int width,int height, bool fullscreen)
{
	// attempt to initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL init success\n";

		int flags = SDL_WINDOW_SHOWN;
		if (fullscreen)
		{
			flags = flags | SDL_WINDOW_FULLSCREEN;
		}

		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos,	width, height, flags);

		if (m_pWindow != 0) // window init success
		{
			std::cout << "window creation success\n";
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (m_pRenderer != 0) // renderer init success
			{
				std::cout << "renderer creation success\n";
			}
			else
			{
				std::cout << "renderer init fail\n";
				return false; // renderer init fail
			}
		}
		else
		{
			std::cout << "window init fail\n";
			return false; // window init fail
		}
	}
	else
	{
		std::cout << "SDL init fail\n";
		return false; // SDL init fail
	}
	std::cout << "init success\n";
	m_bRunning = true; // everything is successful

	TheInputHandler::Instance()->initialiseJoysticks();

	player = std::make_unique<Player>();

	return true;
}

void Game::draw()
{
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

	SDL_SetRenderDrawColor(m_pRenderer, 200, 200, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_pRenderer); // clear the renderer to	the draw color

	stringRGBA(m_pRenderer, TITLE_X, TITLE_Y, "Use arrows or mouse to move point, use Esc to exit.", 255, 255, 255, 255);

	// draw perimeter
	boxRGBA(m_pRenderer, 0, 0, SCREEN_WIDTH, BORDER_WIDTH,
		0,0,255,255);
	boxRGBA(m_pRenderer, 0, 0, BORDER_WIDTH, SCREEN_HEIGHT,
		0, 0, 255, 255);
	boxRGBA(m_pRenderer, 0, SCREEN_HEIGHT - BORDER_WIDTH, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, 0, 255, 255);
	boxRGBA(m_pRenderer, SCREEN_WIDTH - BORDER_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
		0, 0, 255, 255);

	if (player) {
		player->update();
		player->draw(m_pRenderer);
	}
	//filledPolygonRGBA(m_pRenderer, verts1.data(), verts2.data(), 3 , 255, 0, 255, 255);
	SDL_RenderPresent(m_pRenderer); // flip to the screen
}

void Game::clean()
{
	TheInputHandler::Instance()->clean();

	std::cout << "cleaning game\n";
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	SDL_Quit();
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();
}