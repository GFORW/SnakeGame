#include "game.h"
#include <random>
#include <time.h>

Game::Game() : Engine() // 
{
	ptrSnake = std::move(std::make_unique<Snake>());
	ptrSnake->dir = Direction::down;
	GameSpeed = 150;
	WIN_CONDITION = 100;
	play = TRUE;
	drawMenu();
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
	drawFPS();
	if (MENU == 1)
	{
		MENU++;
		return;
	}
	else if (MENU == 2)
	{
		std::cin.ignore();
		MENU++;
		for (int i = 0; i < ScreenX; i++)
			for (int j = 0; j < ScreenY; j++)
				SetChar(i, j, L' ');
	}

	drawTable();
	drawApple();
	drawSnake();
	if (GAME_OVER)
	{
		GameOver();
		drawScore();
		return;
	}
	if (WON)
	{
		Win();
		drawScore();
		return;
	}

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
	for (size_t i=0, x = ScreenX - 8; i < score.size();x++,i++)
	{
			SetChar(x, 5, score.at(i));
	}
	drawScore();
}

void Game::drawApple()
{
	if (!APPLE_PLACED)
	{
		srand((unsigned int)time(nullptr));
		short apple_x = 1 + (rand() % (ScreenX - 13)); 	// get random accessable place
		srand((unsigned int)time(nullptr));
		short apple_y = 1 + (rand() % (ScreenY - 2));

		COORD check{ apple_x,apple_y };
		while (GetChar(apple_x, apple_y) == snake)
		{
			srand((unsigned int)time(NULL));
			int apple_x = 1 + rand() % ScreenX - 12;
			srand((unsigned int)time(NULL));
			int apple_y = 1 + rand() % ScreenY - 1;
		}
		SetChar(apple_x, apple_y, apple);
		APPLE_PLACED = TRUE;
	}
}

void Game::drawScore()
{
	std::wstring scr = std::to_wstring(SCORE);
	for (size_t i = 0, x = ScreenX - 7; i < scr.size(); x++, i++)
	{
		SetChar(x, 6, scr[i]); 
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

void Game::drawFPS()
{
	std::string title = "SnakeGame      FPS : ";
	title += std::to_string(FPS);
	SetConsoleTitleA(title.c_str());
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
	}

	if (GetChar(head.X, head.Y) == bounds)
	{
		GAME_OVER = TRUE;
	}

	if (GetChar(head.X, head.Y) == apple) // redo logic for score updare
	{
		ptrSnake->addPiece();
		APPLE_PLACED = FALSE;
		SCORE++;
		drawScore();
		ChangeSpeed(10);
		if (SCORE == WIN_CONDITION)
			WON = TRUE;
	}
	ptrSnake->ptrBody->at(0) = head; //  step
}

void Game::drawSnake()
{

	Collision();
	move();
	if (GAME_OVER)
		return;
	for (auto piece : *ptrSnake->ptrOldBody.get())  	// delete old snake
	{
		SetChar(piece.X, piece.Y, L' ');
	}

	for (auto piece : *ptrSnake->ptrBody.get())  // draw new snake
	{
		SetChar(piece.X, piece.Y, snake);
	}

}

void Game::drawMenu()
{
	std::string text = "SNAKE GAME";
	std::string text1 = "TO WIN - EAT " + std::to_string(WIN_CONDITION) + " APPLES";
	std::string text2 = "DO NOT EAT BORDERS OR YOUR TAIL";
	std::string text3 = "PRESS ANY BUTTON TO START";

	for (size_t i = 0, x = int(ScreenX / 2) - text.size(); i < text.size(); x++, i++)
	{
		SetChar(x, int(ScreenY / 2), text.at(i));
	}
	for (size_t i = 0, x = int(ScreenX / 2) - text1.size() / 2; i < text1.size(); x++, i++)
	{
		SetChar(x, int(ScreenY / 2) + 1, text1.at(i));
	}
	for (size_t i = 0, x = int(ScreenX / 2) - text2.size() / 2; i < text2.size(); x++, i++)
	{
		SetChar(x, int(ScreenY / 2) + 2, text2.at(i));
	}
	for (size_t i = 0, x = int(ScreenX / 2) - text3.size() / 2; i < text3.size(); x++, i++)
	{
		SetChar(x, int(ScreenY - 2), text3.at(i));
	}

	MENU++;
}

void Game::GameOver()
{
	play = FALSE;
	std::string gm_ov = "GAME OVER";
	for (size_t i = 0, x = int(ScreenX/2)- gm_ov.size(); i < gm_ov.size(); x++, i++)
	{
		SetChar(x, int(ScreenY/2), gm_ov.at(i));
	}
};

void Game::Win()
{
	play = FALSE;
	std::string gm_ov = "YOU WIN";
	for (size_t i = 0, x = int(ScreenX / 2) -12 +gm_ov.size()/2; i < gm_ov.size(); x++, i++)
	{
		SetChar(x, int(ScreenY / 2), gm_ov.at(i));
	}
	std::string congr_str = "CONGRATULATIONS";
	for (size_t i = 0, x = int(ScreenX / 2) - 12; i < congr_str.size(); x++, i++)
	{
		SetChar(x, int(ScreenY / 2)-1, congr_str.at(i));
	}

}