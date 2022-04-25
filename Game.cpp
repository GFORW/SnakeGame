#include "game.h"
#include <random>
#include <time.h>
Game::Game() : Engine() // 
{
	ptrSnake = std::move(std::make_unique<Snake>());
	ptrSnake->dir = Direction::down;
	APPLE_PLACED = FALSE;
}

Game::~Game()
{
}

void Game::KeyPressed(int btnCode)
{
	if (btnCode == 119) //w
	{
		ptrSnake->dir = Direction::up;
	}
	else if (btnCode == 115)//s
	{
		ptrSnake->dir = Direction::down;
	}
	else if (btnCode == 97) //a
	{
		ptrSnake->dir = Direction::left;
	}
	else if (btnCode == 100) //d
	{
		ptrSnake->dir = Direction::right;
	}
}

void Game::UpdateF()
{

	ptrSnake->move();
	drawTable();
	drawApple();
	drawSnake();

	//render map
	//render score
	//render snake
	//render apple
}

void Game::drawTable()
{
	// fill bounds
	for (int y = 0; y < ScreenY; y++)
	{
		for (int x = 0; x < ScreenX; x++)
		{
			if ((y == 0) || (x == 0) || x == ScreenX-1 || y == ScreenY-1)
				SetChar(x, y, bounds);
			if ((ScreenX - x) == 11)  // make score part dynamicly adjustable
				SetChar(x, y, bounds);
		}
	}
	std::string score = "SCORE";
	for (int i=0, x = ScreenX - 8; i < score.size();x++,i++)
	{
			SetChar(x, 5, score.at(i));
	}
}

void Game::drawApple()
{
	if (!APPLE_PLACED)
	{
		srand(time(NULL));
		int apple_x = 1 + rand() % (ScreenX - 12); 	// get random accessable place
		srand(time(NULL));
		int apple_y = 1 + rand() % (ScreenY - 1);

		COORD check{ apple_x,apple_y };
		while (GetChar(apple_x, apple_y) == snake)
		{
			srand(time(NULL));
			int apple_x = 1 + rand() % ScreenX - 12;
			srand(time(NULL));
			int apple_y = 1 + rand() % ScreenY - 1;
		}
		SetChar(apple_x, apple_y, apple);
		APPLE_PLACED = TRUE;
	}
}

void Game::Collision()
{
	Coord* head = ptrSnake->ptrBody->data();
	if (GetChar(head->x, head->y) == bounds)
		GAME_OVER = TRUE;
	else if (GetChar(head->x, head->y) == apple)
	{
		ptrSnake->addPiece();
		APPLE_PLACED = FALSE;

	}
}

void Game::drawSnake()
{
	Collision();

	for (auto piece : *ptrSnake->ptrOldBody.get())  	// delete old snake
	{
		SetChar(piece.x, piece.y, L' ');
	}

	for (auto piece : *ptrSnake->ptrBody.get())  // draw new snake
	{
		SetChar(piece.x, piece.y, snake);
	}
}

void Game::GameOver()
{
	for (int i = 0; i < ScreenX; i++)
		for (int j = 0; j < ScreenY; j++)
			SetChar(i,j,L' ');

	std::string gm_ov = "GAME OVER";
	for (int i = 0, x = int(ScreenX/2)- gm_ov.size(); i < gm_ov.size(); x++, i++)
	{
		SetChar(x, int(ScreenY/2), gm_ov.at(i));
	}

	std::string score = "SCORE";
	for (int i = 0, x = int(ScreenX / 2) - gm_ov.size()+2; i < score.size(); x++, i++)
	{
		SetChar(x, int(ScreenY / 2)+1, score.at(i));
	}

};



