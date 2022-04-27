#include "game.h"
#include <random>
#include <time.h>


#define MiddleX (unsigned int)((ScreenX-1)/2)
#define MiddleY (unsigned int)((ScreenY-1)/2)

#define ScorePanelEndX  (ScreenX-1)
#define ScorePanelStartX  (ScreenX - 11)
#define ScorePanelMiddleX (ScorePanelStartX + (ScorePanelEndX-ScorePanelStartX)/2)
#define ScorePanelMiddleY (unsigned int)((ScreenY-1)/2)

#define MiddleBoardX (1 + (unsigned int)(ScorePanelStartX/2))
#define MiddleBoardY (1 + (unsigned int)((ScreenY-2)/2))


Game::Game() : Engine() // 
{
	ptrSnake = std::move(std::make_unique<Snake>());
	ptrSnake->dir = Direction::down;
	GameSpeed = 150;
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
			if (x == ScorePanelStartX)  // make score part dynamicly adjustable
				SetChar(x, y, bounds);
		}
	}
	std::string score = "SCORE";
	for (size_t i=0, x = ScorePanelMiddleX -score.size()/2; i < score.size();x++,i++)
	{
			SetChar(x, ScorePanelMiddleY, score.at(i));
	}
	drawScore();
}

void Game::drawApple()
{
	if (!APPLE_PLACED)
	{
		auto seed = std::chrono::system_clock::now();
		srand(std::chrono::system_clock::to_time_t(seed));
		short apple_x = 1 + (rand() % ScorePanelStartX); 	// get random accessable place
		seed = std::chrono::system_clock::now();
		std::chrono::system_clock::to_time_t(seed);
		short apple_y = 1 + (rand() % (ScreenY - 2));

		COORD check{ apple_x,apple_y };
		while (GetChar(apple_x, apple_y) == snake)
		{
			seed = std::chrono::system_clock::now();
			srand(std::chrono::system_clock::to_time_t(seed));
			int apple_x = 1 + rand() % ScorePanelStartX;
			seed = std::chrono::system_clock::now();
			srand(std::chrono::system_clock::to_time_t(seed));
			int apple_y = 1 + rand() % (ScreenY - 2);
		}
		SetChar(apple_x, apple_y, apple);
		APPLE_PLACED = TRUE;
	}
}

void Game::drawScore()
{
	std::wstring scr = std::to_wstring(SCORE);
	for (unsigned int i = 0, x = ScorePanelMiddleX; i < scr.size(); x++, i++)
	{
		SetChar(x, ScorePanelMiddleY+1, scr[i]);
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
	std::string text1 = " - TO WIN - EAT " + std::to_string(WIN_CONDITION) + " APPLES";
	std::string text2 = " - DO NOT EAT BORDERS OR YOUR TAIL";
	std::string text3 = " - EACH APPLE INCREASES YOUR SPEED";
	std::string text4 = "PRESS ANY BUTTON TO START";

	for (size_t i = 0, x = MiddleX - text.size() /2; i < text.size(); x++, i++)
	{
		SetChar(x, MiddleY -5, text.at(i));
	}
	for (size_t i = 0, x = MiddleX + text.size() - text1.size() ; i < text1.size(); x++, i++)
	{
		SetChar(x, MiddleY , text1.at(i));
	}
	for (size_t i = 0, x = MiddleX + text.size()  - text1.size(); i < text2.size(); x++, i++)
	{
		SetChar(x, MiddleY + 1, text2.at(i));
	}
	for (size_t i = 0, x = MiddleX + text.size() - text1.size(); i < text3.size(); x++, i++)
	{
		SetChar(x, MiddleY + 2, text3.at(i));
	}

	for (size_t i = 0, x = MiddleX - text4.size() / 2; i < text4.size(); x++, i++)
	{
		SetChar(x, ScreenY -2, text4.at(i));
	}

	MENU++;
}

void Game::GameOver()
{
	play = FALSE;
	std::string gm_ov = "GAME OVER";
	for (unsigned int i = 0, x = MiddleBoardX - gm_ov.size()/2; i < gm_ov.size(); x++, i++)
	{
		SetChar(x, MiddleBoardY, gm_ov.at(i));
	}
};

void Game::Win()
{
	play = FALSE;
	std::string congr_str = "CONGRATULATIONS";
	for (size_t i = 0, x = MiddleBoardX - congr_str.size()/2; i < congr_str.size(); x++, i++)
	{
		SetChar(x, MiddleBoardY , congr_str.at(i));
	}

	std::string win_str = "YOU WIN";
	for (size_t i = 0, x = MiddleBoardX - congr_str.size()/4; i < win_str.size(); x++, i++)
	{
		SetChar(x, MiddleBoardY +1, win_str.at(i));
	}

}