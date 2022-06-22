#pragma once
#include "game.h"

int main()
{
	Game* const SnakeGame = new Game();
	SnakeGame->Run();
	delete SnakeGame;
	return 0;
}