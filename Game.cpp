#include <iostream>
#include <fstream>
#include <SDL2_gfxPrimitives.h>
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "GameObject.h"
#include "Polygon.h"

Game* Game::s_pInstance = 0;

Game::Game() : m_pWindow(0), m_pRenderer(0), m_bRunning(false)
{

}

Game::~Game()
{
	m_pWindow = 0;
	m_pRenderer = 0;

	m_gameObjs.clear();
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

	LoadPolygonsFromFile(FILENAME);
	m_player = std::make_unique<Player>();

	return true;
}

void Game::draw()
{
	SDL_SetRenderDrawColor(m_pRenderer, 200, 200, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_pRenderer); // clear the renderer to	the draw color
	
	//SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	for (std::vector<GameObject*>::size_type i = 0; i < m_gameObjs.size(); ++i)
	{
		m_gameObjs[i]->draw(m_pRenderer);
	}

	if (m_player) {
		m_player->update();
		m_player->draw(m_pRenderer);
	}

	stringRGBA(m_pRenderer, TITLE_X, TITLE_Y, "Use arrows to move a circle, use Esc to exit.", 255, 255, 255, 255);
	
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

void Game::LoadPolygonsFromFile(const std::string fileName)//load a vertices data from the file to build polygons
{
	std::ifstream myFile("coords.txt");
	if (myFile.is_open())
	{
		while (myFile.good())
		{
			std::string line;
			bool part(false);
			std::vector<int> x_vect;
			std::vector<int> y_vect;
			std::getline(myFile, line);
			std::string str_x(""), str_y("");

			for (unsigned int i = 0; i < line.size(); ++i)
			{
				if (!part)
				{
					if (line[i] != ' ')
					{
						str_x.push_back(line[i]);
					}
					else
					{
						part = !part;
					}
				}
				else
				{
					if (line[i] != ',')
					{
						str_y.push_back(line[i]);
					}
					else
					{
						part = !part;
						int x{ std::stoi(str_x) };
						int y{ std::stoi(str_y) };
						x_vect.push_back(x);
						y_vect.push_back(y);
						str_x = "";
						str_y = "";
					}
				}
			}
			GameObject* poly = new Polygon(x_vect, y_vect);
			m_gameObjs.push_back(poly);
		}
	}
}