#pragma once
#include "Engine.h"
#include "Snake.h"

#define bounds L'#'
#define snake L'x'
#define apple L'a'



class Game : public Engine
{
public:
	Game();
	~Game();

	virtual void KeyPressed(int btnCode);
	virtual void Update();
	void Collision();

	void drawMenu();
	void drawSnake();
	void drawTable();
	void drawApple();
	void drawScore();
	void drawFPS();
	void move();

	void GameOver();
	void Win();

	std::unique_ptr<Snake> ptrSnake;

	int MENU = 0;
	bool GAME_OVER = FALSE;
	bool WON = FALSE;
	int SCORE = 0;
	int WIN_CONDITION;
	bool APPLE_PLACED = FALSE;
};