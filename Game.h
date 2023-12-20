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
	void update() {}
	void handleEvents();
	void clean();
	// a function to access the private running variable
	bool running() { return m_bRunning; }
	void quit() { m_bRunning = false; }

	void setPlayerVelocity(const Vector2D vec);
	void setPlayerInertia();
	bool getCollisionDetect();
	bool getInertia();

private:
	bool m_bRunning;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	static Game* s_pInstance;

	std::unique_ptr<Player> m_player;
	std::vector<GameObject*>  m_gameObjs;
	Bonus* m_bonus;

	//random generator
	std::mt19937 m_random_engine;

	Game();
	~Game();

	Game(const Game&);
	Game& operator=(const Game&);

	void LoadPolygonsFromFile(const std::string fileName);
	void CreateBonus();
};
typedef Game TheGame;