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
	virtual void GameOver();

	void Collision();

	void drawSnake();
	void drawTable();
	void drawApple();
	void drawScore();
	void move();

	std::unique_ptr<Snake> ptrSnake;
	
	int SCORE;
	bool APPLE_PLACED;
};