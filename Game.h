#pragma once
#include <CnsFramework.h>
#include "Snake.h"

#define bounds L'#'
#define snake L'x'
#define apple L'a'


class Game : public CnsFramework
{
public:
	Game();
	~Game();
	void KeyPressed(int btnCode);
	void Update(GameState * State);
	void Collision();
	bool Handle_Events();
	void drawMenu();
	void drawSnake();
	void drawTable();
	void drawApple();
	void drawScore(GameState * state);
	void drawFPS();

	void move();

	void GameOver();
	void Win();

	std::unique_ptr<Snake> ptrSnake;
	GameState game;
	GameState menu;
	GameState game_over;
	GameState win;

private:
	int MENU = 0;
	bool GAME_OVER = FALSE;
	bool WIN = FALSE;
	int SCORE = 0;
	int WIN_CONDITION = 10;
	bool APPLE_PLACED = FALSE;
};

