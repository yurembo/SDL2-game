#pragma once

#include <memory>
#include <vector>
#include <string>
#include <SDL.h>
#include <random>

class Player;
class GameObject;
class Vector2D;
class Bonus;
class Polygon;
class Enemy;

class Game
{
public:
	static Game* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Game();
			return s_pInstance;
		}

		return s_pInstance;
	}

	bool init(const char* title, int xpos, int ypos, int width, int	height, bool fullscreen);
	void draw();
	void showScore(SDL_Renderer* m_pRenderer);
	void drawFieldOfView(std::vector<Vector2D>& coords);
	void update() {}
	void handleEvents();
	void clean();
	// a function to access the private running variable
	bool running() { return m_bRunning; }
	void quit() { m_bRunning = false; }

	void setPlayerVelocity(const Vector2D vec);
	void setPlayerInertia();
	bool getInertia();

	void readValueFromFileToChooseControlDevice();//0 - keyboard, 1 - gamepad

	friend bool operator==(const SDL_Rect& left, const SDL_Rect& right);

private:
	Game();
	~Game();
	Game(const Game&);
	Game& operator=(const Game&);

	bool m_bRunning;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	static Game* s_pInstance;

	std::unique_ptr<Player> m_player;
	std::vector<GameObject*>  m_gameObjs;
	std::vector<Enemy*> m_Enemys;
	Bonus* m_bonus;

	//random generator
	std::mt19937 m_random_engine;

	void LoadPolygonsFromFile(const std::string fileName);
	SDL_Rect findFreeSpace();
	void CreateBonus();
	void CreateEnemies();
};
typedef Game TheGame;