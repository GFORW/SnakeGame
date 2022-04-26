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
	GameSpeed = 150;
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


void Game::move()  
{
	// move tail
	for (size_t i = 1; i < ptrSnake->ptrBody->size(); i++)
	{
		ptrSnake->ptrBody->at(i) = ptrSnake->ptrOldBody->at(i - 1);
	}
}



void Game::Collision()
{
	COORD head{ ptrSnake->ptrBody->at(0) };
	COORD neck{ ptrSnake->ptrBody->at(1) };
	(*ptrSnake->ptrOldBody) = (*ptrSnake->ptrBody);

	switch (ptrSnake->dir)
	{
	case left:
		head.X -= 1;
		break;
	case up:
		head.Y -= 1;
		break;
	case right:
		head.X += 1;
		break;
	case down:
		head.Y += 1;
		break;
	}

	if ((head.X == neck.X) && (head.Y == neck.Y)) // self-turning;
	{
		ptrSnake->dir = ptrSnake->prev_dir;
		Collision();
		return;
	}
	else if (GetChar(head.X, head.Y) == snake) //self-eating
	{
		GAME_OVER = TRUE;
		return;
	}

	if (GetChar(head.X, head.Y) == bounds)
	{
		GAME_OVER = TRUE;
		return;
	}

	if (GetChar(head.X, head.Y) == apple)
	{
		ptrSnake->addPiece();
		APPLE_PLACED = FALSE;
		SCORE++;
		ChangeSpeed(10);
	}
	ptrSnake->ptrBody->at(0) = head; //  step
}

void Game::drawSnake()
{
	Collision();
	move();
	for (auto piece : *ptrSnake->ptrOldBody.get())  	// delete old snake
	{
		SetChar(piece.X, piece.Y, L' ');
	}

	for (auto piece : *ptrSnake->ptrBody.get())  // draw new snake
	{
		SetChar(piece.X, piece.Y, snake);
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

