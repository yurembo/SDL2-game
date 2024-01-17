#include <iostream>
#include "const.h"
#include "Game.h"

const int FPS = 60;
const int DELAY_TIME = 1000 / FPS;

int main(int argc, char* argv[])
{
	Uint32 frameStart, frameTime;

	bool b = TheGame::Instance()->init(GAME_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
	if (b) 
	{
		while (TheGame::Instance()->running())
		{
			frameStart = SDL_GetTicks();

			TheGame::Instance()->handleEvents();
			TheGame::Instance()->update();
			TheGame::Instance()->draw();

			frameTime = SDL_GetTicks() - frameStart;
			if (frameTime < DELAY_TIME)
			{
				SDL_Delay((int)(DELAY_TIME - frameTime));
			}
		}
	} else std::cout << "cannot init SDL" << std::endl;

	TheGame::Instance()->clean();

	return 0;
}