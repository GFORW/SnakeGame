#pragma once
#include "game.h"


#define MiddleX ((ScreenX-1)/2)
#define MiddleY ((ScreenY-1)/2)

#define ScorePanelEndX  (ScreenX-1)
#define ScorePanelStartX  (ScorePanelEndX - 11)
#define ScorePanelMiddleX (ScorePanelStartX + (ScorePanelEndX-ScorePanelStartX)/2)
#define ScorePanelMiddleY ((ScreenY-1)/2)

#define MiddleBoardX (1 + (ScorePanelStartX/2))
#define MiddleBoardY (1 + ((ScreenY-2)/2))


Game::Game(const int& Xsize, const int& Ysize, const std::chrono::nanoseconds tick_ms) : CnsFramework(Xsize,Ysize,tick_ms) // 
{
	game = new GameState(ScreenX, ScreenY, "game");
	menu = new GameState(ScreenX, ScreenY, "menu");
	game_over = new GameState(ScreenX, ScreenY, "game_over");
	win = new GameState(ScreenX, ScreenY, "win");

	current_state = menu;

	ptrSnake = std::make_unique<Snake>(RANDOM::get_random(3, ScorePanelStartX-3), RANDOM::get_random(3, ScreenY - 4)); // spawn away from the border

	ptrSnake->ptrBody->reserve(WIN_CONDITION);
	ptrSnake->ptrOldBody->reserve(WIN_CONDITION);

	drawMenu();
	drawTable();
	drawApple();
	drawScore(game);
}

Game::~Game()
{
	delete game;
	delete menu;
	delete game_over;
	delete win;
}

void Game::KeyPressed(const int& Code)
{
	ptrSnake->prev_dir = ptrSnake->dir;
	if (GetAsyncKeyState('W') & 0x01) //w
	{
		ptrSnake->dir = Direction::up;
	}
	else if (GetAsyncKeyState('S') & 0x01) //s
	{
		ptrSnake->dir = Direction::down;
	}
	else if (GetAsyncKeyState('A') & 0x01) //a
	{
		ptrSnake->dir = Direction::left;
	}
	else if (GetAsyncKeyState('D') & 0x01) //d
	{
		ptrSnake->dir = Direction::right;
	}
}

bool Game::Handle_Events()
{
	if (current_state->name == "game_over")
	{
		if (env & GAME_OVER)
		{
			env &= ~GAME_OVER;
			return true; // do another loop for render
		}
		std::cin.ignore();
		return false;
	}
	if (current_state->name == "win")
	{
		if (env & WIN)
		{
			env &= ~WIN;
			return true; // do another loop for render
		}
		std::cin.ignore();
		return false;
	}
	return true;
}
void Game::Update()
{
	const std::string& state_name = current_state->name;
	drawFPS();
	if (state_name == "menu")
	{
		if (env & MENU)
		{
			std::cin.ignore();
			current_state = game;
		}
		else env |= MENU;
		return;
	}
	if (state_name == "game")
	{
		drawTable();
		drawApple();
		drawSnake();
		return;
	}
	if (state_name == "game_over")
	{
		GameOver();
		drawScore(game_over);
		return;
	}
	if (state_name == "win")
	{
		Win();
		drawScore(game);
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
				game->SetChar(x, y, bounds);
			if (x == ScorePanelStartX)  
				game->SetChar(x, y, bounds);
		}
	}
	const std::string score = "SCORE";
	for (size_t i = 0, x = ScorePanelMiddleX - score.size() / 2; i < score.size(); x++, i++)
	{
		game->SetChar(x, ScorePanelMiddleY, score[i]);
	}
	drawScore(game);
}

void Game::drawApple()
{
	if (!(env & APPLE_PLACED))
	{
		COORD APPLE{};
		do
		{
			APPLE.X = RANDOM::get_random(1, ScorePanelStartX);
			APPLE.Y = RANDOM::get_random(1, ScreenY - 2);
		} 
		while (game->GetChar(APPLE.X, APPLE.Y) != L' ');
		game->SetChar(APPLE.X, APPLE.Y, apple);
		env |= APPLE_PLACED;
	}
}

void Game::drawScore(GameState* const state)
{
	const std::wstring scr = std::to_wstring(SCORE);
	for (unsigned int i = 0, x = ScorePanelMiddleX; i < scr.size(); x++, i++)
	{
		state->SetChar(x, ScorePanelMiddleY+1, scr[i]);
	}
}

void Game::move()
{
	//move tail
	for (size_t i = 1; i < ptrSnake->ptrBody->size(); i++)
	{
		(*ptrSnake->ptrBody)[i] = (*ptrSnake->ptrOldBody)[i - 1];
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
	COORD head{ (*ptrSnake->ptrBody)[0]};
	COORD neck{};
	if (ptrSnake->ptrBody->size()>1)
		neck = (*ptrSnake->ptrBody)[1];
	
	ptrSnake->ptrOldBody->assign(ptrSnake->ptrBody->begin(), ptrSnake->ptrBody->end());

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
	else if (game->GetChar(head.X, head.Y) == snake) //self-eating
	{
		current_state = game_over;
		env |= GAME_OVER;
	}

	if (game->GetChar(head.X, head.Y) == bounds)
	{
		current_state = game_over;
		env |= GAME_OVER;
	}

	if (game->GetChar(head.X, head.Y) == apple) // redo logic for score update
	{
		ptrSnake->addPiece();
		env &= ~APPLE_PLACED;
		SCORE++;
		drawScore(game);
		if((tick - 10ms)>0ms)
			ChangeTickSpeed(tick - 10ms);
		if (SCORE == WIN_CONDITION)
		{
			env |= WIN;
			current_state = win;
		}
	}
	(*ptrSnake->ptrBody)[0] = head; //  step
}

void Game::drawSnake()
{
	Collision();
	move();
	if (env & GAME_OVER)
		return;
	for (auto& piece : *ptrSnake->ptrOldBody.get())  	// delete old snake
	{
		game->SetChar(piece.X, piece.Y, L' ');
	}
	for (auto& piece : *ptrSnake->ptrBody.get())  // draw new snake
	{
		game->SetChar(piece.X, piece.Y, snake);
	}
}

void Game::drawMenu()
{
	const std::string text = "SNAKE GAME";
	for (size_t i = 0, x = MiddleX - text.size() /2; i < text.size(); x++, i++)
	{
		menu->SetChar(x, MiddleY - 5, text[i]);
	}
	const std::string text1 = " - TO WIN - EAT " + std::to_string(WIN_CONDITION) + " APPLES";
	for (size_t i = 0, x = MiddleX + text.size() - text1.size() ; i < text1.size(); x++, i++)
	{
		menu->SetChar(x, MiddleY , text1[i]);
	}
	const std::string text2 = " - DO NOT EAT BORDERS OR YOUR TAIL";
	for (size_t i = 0, x = MiddleX + text.size()  - text1.size(); i < text2.size(); x++, i++)
	{
		menu->SetChar(x, MiddleY + 1, text2[i]);
	}
	const std::string text3 = " - EACH APPLE INCREASES YOUR SPEED";
	for (size_t i = 0, x = MiddleX + text.size() - text1.size(); i < text3.size(); x++, i++)
	{
		menu->SetChar(x, MiddleY + 2, text3[i]);
	}
	const std::string text4 = "PRESS ANY BUTTON TO START";
	for (size_t i = 0, x = MiddleX - text4.size() / 2; i < text4.size(); x++, i++)
	{
		menu->SetChar(x, ScreenY -2, text4[i]);
	}
}

void Game::GameOver()
{
	const std::string gm_ov = "GAME OVER";
	for (size_t i = 0, x = MiddleBoardX - gm_ov.size()/2; i < gm_ov.size(); x++, i++)
	{
		game_over->SetChar(x, MiddleBoardY, gm_ov[i]);
	}
	const std::string score = "SCORE";
	for (size_t i = 0, x = ScorePanelMiddleX - score.size() / 2; i < score.size(); x++, i++)
	{
		game_over->SetChar(x, ScorePanelMiddleY, score[i]);
	}
	const std::string text = "PRESS ANY BUTTON TO QUIT";
	for (size_t i = 0, x = MiddleX - text.size() / 2; i < text.size(); x++, i++)
	{
		game_over->SetChar(x, ScreenY - 2, text[i]);
	}
	drawScore(game_over);
};

void Game::Win()
{
	const std::string congr_str = "CONGRATULATIONS";
	for (size_t i = 0, x = MiddleBoardX - congr_str.size()/2; i < congr_str.size(); x++, i++)
	{
		win->SetChar(x, MiddleBoardY , congr_str[i]);
	}
	const std::string win_str = "YOU WIN";
	for (size_t i = 0, x = MiddleBoardX - congr_str.size()/4; i < win_str.size(); x++, i++)
	{
		win->SetChar(x, MiddleBoardY +1, win_str[i]);
	}
	const std::string text = "PRESS ANY BUTTON TO QUIT";
	for (size_t i = 0, x = MiddleX - text.size() / 2; i < text.size(); x++, i++)
	{
		win->SetChar(x, ScreenY - 2, text[i]);
	}
	const std::string score = "SCORE";
	for (size_t i = 0, x = ScorePanelMiddleX - score.size() / 2; i < score.size(); x++, i++)
	{
		win->SetChar(x, ScorePanelMiddleY, score[i]);
	}
	drawScore(win);
}
