#include "game.h"

Game::Game() : Engine() // 
{
	ptrSnake = std::move(std::make_unique<Snake>());
	ptrSnake->dir = Direction::down;
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
			if ((ScreenX - x) == 11)
				SetChar(x, y, bounds);
		}
	}
	std::string score = "SCORE";
	for (int i=0, x = ScreenX - 8; i < score.size();x++,i++)
	{
			SetChar(x, 5, score.at(i));
	}
}

bool Game::Collision()
{
	Coord* head = ptrSnake->ptrOldBody->data();
	if (GetChar(head->x, head->y) == bounds)
		return true;
	else
		return false;
}

void Game::drawSnake()
{
	if (Collision())
	{
		GAME_OVER = TRUE;
		return;
	}

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



