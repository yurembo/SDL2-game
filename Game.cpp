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
#include "Enemy.h"

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

	m_Enemys.clear();
}

bool Game::init(const char* title, int xpos, int ypos, int width,int height, bool fullscreen)
{
	// attempt to initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "SDL init success" << std::endl;

		int flags = SDL_WINDOW_SHOWN;
		if (fullscreen)
		{
			flags = flags | SDL_WINDOW_FULLSCREEN;
		}

		// init the window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos,	width, height, flags);

		if (m_pWindow != 0) // window init success
		{
			std::cout << "window creation success" << std::endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (m_pRenderer != 0) // renderer init success
			{
				std::cout << "renderer creation success" << std::endl;
			}
			else
			{
				std::cout << "renderer init fail" << std::endl;
				return false; // renderer init fail
			}
		}
		else
		{
			std::cout << "window init fail" << std::endl;
			return false; // window init fail
		}
	}
	else
	{
		std::cout << "SDL init fail" << std::endl;
		return false; // SDL init fail
	}
	std::cout << "init success" << std::endl;
	m_bRunning = true; // everything is successful

	TheInputHandler::Instance()->initialiseJoysticks();

	LoadPolygonsFromFile(FILENAME);
	m_player = std::make_unique<Player>();

	std::random_device dev;
	m_random_engine.seed(dev());
	CreateBonus();
	CreateEnemies();

	readValueFromFileToChooseControlDevice();

	return true;
}

void Game::draw()
{
	SDL_SetRenderDrawColor(m_pRenderer, 200, 200, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_pRenderer); // clear the renderer to draw color
	
	if (m_player) 
	{
		std::vector<Vector2D> out_intersectDots;
		m_player->ShootRays(m_pRenderer, m_gameObjs, out_intersectDots);
		drawFieldOfView(out_intersectDots);

		for (std::vector<GameObject*>::size_type i = 0; i < m_gameObjs.size(); ++i)
		{
			m_gameObjs[i]->draw(m_pRenderer);			
			Polygon* poly = dynamic_cast<Polygon*>(m_gameObjs[i]);
			if (m_player->checkCollisionWithPolygon(m_pRenderer, poly->getVertexX(), poly->getVertexY()))
			{
				std::cout << "PlayerСollide"  << std::endl;
				Vector2D v { 0, 0 };
				m_player->resolveCollision(v);
			}
			for (auto iter = m_Enemys.begin(); iter != m_Enemys.end(); ++iter)
			{
				Enemy* enemy = dynamic_cast<Enemy*>(*iter);
				if (enemy->checkCollisionWithPolygon(m_pRenderer, poly->getVertexX(), poly->getVertexY()))
				{
					std::cout << "EnemyCollide" << std::endl;
					Vector2D v{ 0, 0 };
					enemy->resolveCollision(v);
				}
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
	
	
		for (auto iter = m_Enemys.begin(); iter != m_Enemys.end(); ++iter)
		{
			Enemy* enemy = dynamic_cast<Enemy*>(*iter);
			if (m_player->getScore() > 0)
			{
				enemy->moveToPlayer(m_player->getPosition());
			}
			enemy->draw(m_pRenderer);
			if (m_player->checkCollisionWithEnemy(m_player->getCollider(), enemy->getCollider()))
			{
				std::cout << "Player collides with Enemy" << std::endl;
				enemy->resolveCollision(m_player->getVelocity());
				m_player->resolveCollision(enemy->getVelocity());
			}
		}
	}

	if (m_bonus) {
		m_bonus->draw(m_pRenderer);
	}

	showScore(m_pRenderer);
	stringRGBA(m_pRenderer, TITLE_X, TITLE_Y, "Use arrows, WASD or left stick on a gamepad to move the circle, press Esc to exit.", 255, 255, 255, 255);
	
	SDL_RenderPresent(m_pRenderer); // flip to the screen
}

void Game::showScore(SDL_Renderer* m_pRenderer)
{
	if (m_player)
	{
		int score = m_player->getScore();
		std::string s = "Score: " + std::to_string(score);
		stringRGBA(m_pRenderer, TITLE_X, SCREEN_HEIGHT - TITLE_Y * 2, s.c_str(), 255, 255, 255, 255);
	}
}

void Game::drawFieldOfView(std::vector<Vector2D>& coords)
{
	std::vector<Sint16>vectorX;
	std::vector<Sint16>vectorY;

	for (auto iter = coords.begin(); iter != coords.end(); ++iter)
	{
		//filledCircleRGBA(m_pRenderer, (int)iter->getX(), (int)iter->getY(), 10, 255, 0, 0, 255);
		//lineRGBA(m_pRenderer, (int)m_player->getPosition().getX(), (int)m_player->getPosition().getY(), (int)iter->getX(), (int)iter->getY(), 255, 0, 0, 255);

		vectorX.push_back((int)iter->getX());
		vectorY.push_back((int)iter->getY());
	}

	filledPolygonRGBA(m_pRenderer, vectorX.data(), vectorY.data(), vectorX.size(), 255, 255, 255, 255);
}

void Game::clean()
{
	TheInputHandler::Instance()->clean();

	std::cout << "cleaning game" << std::endl;
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

bool operator==(const SDL_Rect& left, const SDL_Rect& right)
{
	return (left.x == right.x && left.y == right.y && left.w == right.w && left.h == right.h);
}

SDL_Rect Game::findFreeSpace()
{
	const int m = 100;
	std::uniform_int_distribution<std::mt19937::result_type> x_range(m, SCREEN_WIDTH-m);
	std::uniform_int_distribution<std::mt19937::result_type> y_range(m, SCREEN_HEIGHT-m);

	const SDL_Rect zero{ 0, 0, 0, 0 };
	SDL_Rect returnRect = zero;

	while (returnRect == zero)
	{
		to_begin://label 

 		SDL_Rect rect;
 		rect.x = (int)x_range(m_random_engine) - BONUS_RECT_WIDTH-m;
 		rect.y = (int)y_range(m_random_engine) - BONUS_RECT_WIDTH-m;
 		rect.w = BONUS_RECT_WIDTH * 2+m;
 		rect.h = BONUS_RECT_WIDTH * 2+m;

		for (std::vector<GameObject*>::size_type i = 0; i < m_gameObjs.size(); ++i)
		{
			Polygon* poly = dynamic_cast<Polygon*>(m_gameObjs[i]);
			if (SDL_HasIntersection(&rect, poly->getCollider()) == SDL_TRUE)
			{
				goto to_begin;
			}
		}
		if (SDL_HasIntersection(&rect, &m_player->getCollider()) == SDL_TRUE)
		{
			goto to_begin;
		}
		returnRect = rect;
	}
	return returnRect;
}

void Game::CreateBonus()
{
	SDL_Rect rect = findFreeSpace();
	if (rect.x != 0 && rect.y != 0)
	{
		m_bonus = new Bonus(static_cast<float>(rect.x), static_cast<float>(rect.y));
	}
}

void Game::CreateEnemies()
{
	for (int i = 0; i < ENEMY_COUNT; ++i)
	{
		SDL_Rect rect =	findFreeSpace();
		if (rect.x != 0 && rect.y != 0)
		{
			Enemy* enemy = new Enemy(static_cast<float>(rect.x), static_cast<float>(rect.y));
			m_Enemys.push_back(enemy);
		}
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