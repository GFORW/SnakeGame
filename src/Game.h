#pragma once
#include <CnsFramework.h>
#include "Snake.h"
#include <bitset>
#include <random>
#include <time.h>



constexpr auto bounds = L'#';
constexpr auto snake = L'x';
constexpr auto apple = L'a';


class Game : public CnsFramework
{
public:
	Game();
	~Game();
private:
	void KeyPressed(const int btnCode) const;
	void Update();
	void Collision();
	bool Handle_Events();

	void drawMenu();
	void drawSnake();
	void drawTable();
	void drawApple();
	void drawScore(GameState * const state);
	void drawFPS();

	void move();

	void GameOver();
	void Win();

	int SCORE = 0;
	const int WIN_CONDITION = 10;

	std::unique_ptr<Snake> ptrSnake;

	GameState *  game;
	GameState *  menu;
	GameState *  game_over;
	GameState *  win;


	std::uint8_t env{0b0000u};


};

enum ENV
{
	MENU = (1 << 0),
	GAME_OVER = (1 << 1),
	WIN = (1 << 2),
	APPLE_PLACED = (1 << 3)
};
