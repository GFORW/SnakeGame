#pragma once
#include "CnsFramework/CnsFramework.h"
#include "Snake.h"

#define bounds L'#'
#define snake L'x'
#define apple L'a'



class Game : public CnsFramework
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

private:
	int GameSpeed = 150;
	int MENU = 0;
	bool GAME_OVER = FALSE;
	bool WON = FALSE;
	int SCORE = 0;
	int WIN_CONDITION = 30;
	bool APPLE_PLACED = FALSE;
};