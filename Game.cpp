#include <iostream>
#include <fstream>
#include <SDL2_gfxPrimitives.h>
#include <ctime>
#include "Game.h"
#include "InputHandler.h"
#include "Player.h"
#include "GameObject.h"
#include "Polygon.h"
#include "Bonus.h"

Game* Game::s_pInstance = 0;

Game::Game() : m_pWindow(0), m_pRenderer(0), m_bRunning(false), m_bonus(nullptr)
{

}

Game::~Game()
{
	m_pWindow = 0;
	m_pRenderer = 0;

	m_gameObjs.clear();

	if (m_bonus) 
	{
		delete m_bonus;
		m_bonus = nullptr;
	}
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

	std::random_device dev;
	m_random_engine.seed(dev());
	CreateBonus();

	readValueFromFileToChooseControlDevice();

	return true;
}

void Game::draw()
{
	SDL_SetRenderDrawColor(m_pRenderer, 200, 200, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_pRenderer); // clear the renderer to draw color
	
	if (m_player) 
	{
		for (std::vector<GameObject*>::size_type i = 0; i < m_gameObjs.size(); ++i)
		{
			m_gameObjs[i]->draw(m_pRenderer);			
			Polygon* poly = dynamic_cast<Polygon*>(m_gameObjs[i]);
			if (m_player->checkCollisionWithPolygon(m_pRenderer, poly->getVertexX(), poly->getVertexY()))
			{
				std::cout << "collide";
				m_player->resolveCollision(*poly);
			}
		}
		if ((m_bonus != nullptr) && (m_player->checkCollisionWithBonus(m_player->getCollider(), m_bonus->getCollider())))
		{
			delete m_bonus;
			m_bonus = nullptr;
			CreateBonus();
		}
		m_player->update();
		m_player->draw(m_pRenderer);
	}
	if (m_bonus) {
		m_bonus->draw(m_pRenderer);
	}

	stringRGBA(m_pRenderer, TITLE_X, TITLE_Y, "Use arrows, WASD or left stick on a gamepad to move the circle, press Esc to exit.", 255, 255, 255, 255);
	if (m_player)
	{
		int score = m_player->getScore();
		std::string s = "Score: " + std::to_string(score);
		stringRGBA(m_pRenderer, TITLE_X, SCREEN_HEIGHT - TITLE_Y*2, s.c_str(), 255, 255, 255, 255);
	}
	
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

void Game::setPlayerVelocity(const Vector2D vec)
{
	if (m_player != nullptr)
		m_player->setVelocity(vec);
}

void Game::setPlayerInertia()
{
	if (m_player != nullptr)
		m_player->setInertia();
}

bool Game::getInertia()
{
	bool b = false;
	if (m_player != nullptr)
		b = m_player->getInertia();
	return b;

}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();
}

void Game::LoadPolygonsFromFile(const std::string fileName)//load a vertices data from the file to build polygons
{
	std::ifstream myFile(fileName);
	if (myFile.is_open())
	{
		while (myFile.good())
		{
			std::string line("");
			bool part(false);
			std::vector<int> x_vect(0);
			std::vector<int> y_vect(0);
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

void Game::CreateBonus()
{
	std::uniform_int_distribution<std::mt19937::result_type> x_range(0, SCREEN_WIDTH);
	std::uniform_int_distribution<std::mt19937::result_type> y_range(0, SCREEN_HEIGHT);

	while (m_bonus == nullptr)
	{
		to_begin://label 
		SDL_Rect rect { (int)x_range(m_random_engine), (int)y_range(m_random_engine), BONUS_RECT_WIDTH, BONUS_RECT_WIDTH };
		for (std::vector<GameObject*>::size_type i = 0; i < m_gameObjs.size(); ++i)
		{
			Polygon* poly = dynamic_cast<Polygon*>(m_gameObjs[i]);

			if (SDL_HasIntersection(&rect, poly->getCollider()) == SDL_TRUE)
			{
				goto to_begin;
			}
		}
		m_bonus = new Bonus(static_cast<float>(rect.x), static_cast<float>(rect.y));
	}
}


void Game::readValueFromFileToChooseControlDevice()//0 - keyborad, 1 - gamepad
{
	std::ifstream myFile("gamepad.txt");
	if (myFile.is_open())
	{
		std::string line("");
		std::getline(myFile, line);
		if (line == "1") 
		{
			if (m_player != nullptr)
			{
				m_player->pickUpGamePad();
			}
		}
	}
}