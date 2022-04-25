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
	virtual void UpdateF();
	virtual void GameOver();

	bool Collision();

	void drawSnake();
	void drawTable();

	std::unique_ptr<Snake> ptrSnake;
	
};