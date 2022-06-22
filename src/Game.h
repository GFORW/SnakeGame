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
	Game(const int& Xsize = 50, const int& Ysize = 25, const std::chrono::nanoseconds tick_ms = 160ms);
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
	void drawFPS();

	void drawScore(GameState* const state);

	void move();

	void GameOver();
	void Win();

	int SCORE{};
	const int WIN_CONDITION{ 10 };

	std::uint8_t env{ 0b0000u };

	std::unique_ptr<Snake> ptrSnake;

	GameState *  game;
	GameState *  menu;
	GameState *  game_over;
	GameState *  win;
};

enum ENV
{
	MENU = (1 << 0),
	GAME_OVER = (1 << 1),
	WIN = (1 << 2),
	APPLE_PLACED = (1 << 3)
};

namespace
{
	namespace RANDOM
	{
		std::random_device rd;
		std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
		std::mt19937 rEng{ ss };

		inline int get_random(const int& min, const int& max)
		{
			std::uniform_int_distribution<int> distr1{ min, max };
			return distr1(rEng);
		};
	}
}
