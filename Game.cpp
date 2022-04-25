#include "game.h"
#include <random>
#include <time.h>


Game::Game() : Engine() // 
{
	SetConsoleTitleA("SnakeGame");
	ptrSnake = std::move(std::make_unique<Snake>());
	ptrSnake->dir = Direction::down;
	APPLE_PLACED = FALSE;
	SCORE = 0;
	drawTable();

}

Game::~Game()
{
}

void Game::KeyPressed(int btnCode)
{
	ptrSnake->prev_dir = ptrSnake->dir;
	if ((btnCode == int('W')) || (btnCode == int('w'))) //w
	{
		ptrSnake->dir = Direction::up;
	}
	else if ((btnCode == int('S')) || (btnCode == int('s')))//s
	{
		ptrSnake->dir = Direction::down;
	}
	else if ((btnCode == int('A')) || (btnCode == int('a'))) //a
	{
		ptrSnake->dir = Direction::left;
	}
	else if ((btnCode == int('D')) || (btnCode == int('d'))) //d
	{
		ptrSnake->dir = Direction::right;
	}
}

void Game::Update()
{
	ptrSnake->move();
	drawApple();
	drawSnake();
	drawScore();
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
		int apple_x = 1 + (rand() % (ScreenX - 13)); 	// get random accessable place
		srand(time(NULL));
		int apple_y = 1 + (rand() % (ScreenY - 2));

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

void Game::drawScore()
{
	std::wstring scr = std::to_wstring(SCORE);

	if (GAME_OVER)
	{
		int y = int(ScreenY / 2) + 2;
		int x = int(ScreenX / 2) - 10 + 5;
		for (int i = 0; i < scr.size(); i++)
			SetChar(x, y, scr[i]);
	}
	else
	{
		for (int i = 0, x = ScreenX - 7; i < scr.size(); x++, i++)
		{
			SetChar(x, 6, scr[i]);  // possible overlfow with big nunmbers
		}

	}
}

void Game::Collision()
{

	Coord* head = ptrSnake->ptrBody->data();
	Coord* neck = &ptrSnake->ptrBody->at(1);

	if (GetChar(head->x, head->y) == bounds)
	{
		GAME_OVER = TRUE;
		return;
	}


	else if (GetChar(head->x, head->y) == apple)
	{
		ptrSnake->addPiece();
		APPLE_PLACED = FALSE;
		SCORE++;
		return;
	}


	switch (ptrSnake->dir)    // forbid turn inside snake and check for self eating
	{
	case left:
		if ((head->x - 1 == neck->x) && (head->y == neck->y))
		{
			ptrSnake->dir = ptrSnake->prev_dir;
			return;
		}
		if (GetChar(head->x - 1, head->y) == snake)
			GAME_OVER = TRUE;
		break;
	case up:
		if ((head->x == neck->x) && (head->y - 1 == neck->y))
		{
			ptrSnake->dir = ptrSnake->prev_dir;
			return;
		}
		if (GetChar(head->x , head->y-1) == snake)
			GAME_OVER = TRUE;
		break;
	case right:
		if ((head->x + 1 == neck->x) && (head->y == neck->y))
		{
			ptrSnake->dir = ptrSnake->prev_dir;
			return;
		}
		if (GetChar(head->x + 1, head->y) == snake)
			GAME_OVER = TRUE;
		break;
	case down:
		if ((head->x  == neck->x) && (head->y + 1 == neck->y))
		{
			ptrSnake->dir = ptrSnake->prev_dir;
			return;
		}
		if (GetChar(head->x, head->y+1) == snake)
			GAME_OVER = TRUE;
		break;
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
	drawScore();
};



