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

	void drawSnake();
	void drawTable();
	void drawApple();
	void drawScore();
	void move();

	void GameOver();
	void Win();

	std::unique_ptr<Snake> ptrSnake;

	bool MENU = 0;
	bool GAME_OVER = 0;
	bool WON = 0;
	int SCORE = 0;
	int WIN_CONDITION;
	bool APPLE_PLACED;
};