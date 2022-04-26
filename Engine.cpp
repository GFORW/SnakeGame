#pragma once
#include "Engine.h"
#include <thread>
#include <chrono>
#include <conio.h>
#include <time.h>

Engine::Engine(int X, int Y) : ScreenX(X), ScreenY(Y)
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	X += 3;
	ScreenX += 3;
	// if x,y > max size;
	_CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole, &info);
	SMALL_RECT windowMaxSize = { 0, 0, info.dwMaximumWindowSize.X - 1, info.dwMaximumWindowSize.Y - 1 };
	if (X > info.dwMaximumWindowSize.X - 1)
	{
		X = info.dwMaximumWindowSize.X;
		ScreenX = X;
	}
	if (Y > info.dwMaximumWindowSize.Y - 1)
	{
		Y = info.dwMaximumWindowSize.Y;
		ScreenY = Y;
	}

	WindowSize = { 0, 0, (SHORT)(ScreenX - 1), (SHORT)(ScreenY - 1) };
	windowBufSize = { (SHORT)ScreenX   , (SHORT)ScreenY };

	//prevent window resizing
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// disable text selection
	{
		DWORD mode;
		GetConsoleMode(hConsoleIn, &mode);
		mode &= ENABLE_EXTENDED_FLAGS;
		if (!SetConsoleMode(hConsoleIn, mode))
		{
			std::cout << "SetConsoleMode failed" << GetLastError() << std::endl;
		}
	}
	// set font 
	{
		CONSOLE_FONT_INFOEX ConsoleFontInfo;
		GetCurrentConsoleFontEx(hConsole, FALSE , &ConsoleFontInfo); 
		ConsoleFontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
		ConsoleFontInfo.FontFamily = 72;
		wcscpy_s(ConsoleFontInfo.FaceName, L"Terminal");
		ConsoleFontInfo.dwFontSize = COORD{ 8,12 };
		SetCurrentConsoleFontEx(hConsole, FALSE, &ConsoleFontInfo);
	}

	// turn off cursor
	{
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = 0;
		cursorInfo.dwSize = 1;
		if (!SetConsoleCursorInfo(hConsole, &cursorInfo))
		{
			std::cout << "SetConsoleCurInfo failed" << GetLastError() << std::endl;
		}
	}


	// set window params
	if (!SetConsoleWindowInfo(hConsole, TRUE, &WindowSize))
	{
		std::cout << "SetConsoleWindowInfo failed with error " << GetLastError() << std::endl;
		std::cin.ignore();
	}
	if (!SetConsoleScreenBufferSize(hConsole, windowBufSize))
	{
		std::cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << std::endl;
		std::cin.ignore();
	}


	// fill up array
	{
		Screen.resize(ScreenX, std::vector<wchar_t>(ScreenY, L' '));

		for (int i = 0; i < ScreenX; i++)
			for (int j = 0; j < ScreenY; j++)
				Screen.at(i).at(j) = L' ';
	}
}

Engine::~Engine()
{

}

void Engine::Render()
{
	WORD attr{};
	DWORD Chars;
	
	// Draw Screen
	for (int i = 0; i < ScreenX; i++)
		for (int j = 0; j < ScreenY; j++)
		{
			COORD pos = { i,j };

			FillConsoleOutputCharacter(hConsole, Screen.at(i).at(j), 1, pos, &Chars);
		}
}

void Engine::SetChar(int x, int y, wchar_t c)  
{
	Screen.at(x).at(y) = c;
}

wchar_t Engine::GetChar(int x, int y)
{
	return Screen.at(x).at(y);
}

void Engine::Run()
{

	while (1)
	{
		Update();
		if (GAME_OVER)
		{
			GameOver();
			Render();
			break;
		}	
		Render();

		if (_kbhit())
		{
			KeyPressed(_getch());
			if (!FlushConsoleInputBuffer(hConsoleIn))
				std::cout << "FlushConsoleInputBuffer failed with error " << GetLastError();
		}

		Sleep(GameSpeed);
	}
	std::cin.get();
}
void Engine::ChangeSpeed(int sec)
{
	GameSpeed += sec;
}