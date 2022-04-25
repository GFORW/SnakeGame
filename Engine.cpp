#pragma once
#include "Engine.h"
#include <thread>
#include <chrono>
#include <conio.h>
#include <algorithm>
#include <time.h>
#include <assert.h>
#include <strsafe.h>
#include <array>

Engine::Engine(int X, int Y) : ScreenX(X), ScreenY(Y)
{
	WindowSize = { 0, 0, (SHORT)ScreenX, (SHORT)ScreenY};
	windowBufSize = { (SHORT)ScreenX  , (SHORT)ScreenY};

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	// set window params
	if (!SetConsoleWindowInfo(hConsole, TRUE, &WindowSize))
	{
		std::cout << "SetConsoleWindowInfo failed with error " << GetLastError() << std::endl;
	}
	if (!SetConsoleScreenBufferSize(hConsole, windowBufSize))
	{
		std::cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << std::endl;
	}

	// turn off cursor
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	if (!SetConsoleCursorInfo(hConsole, &cursorInfo) )
	{
		std::cout <<  "SetConsoleCurInfo failed" << GetLastError() << std::endl;
	}

	//prevent window resizing
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	// disable text selection
	DWORD mode;
	GetConsoleMode(hConsoleIn, &mode);
	mode &= ENABLE_EXTENDED_FLAGS;
	if (!SetConsoleMode(hConsoleIn, mode))
	{
		std::cout << "SetConsoleMode failed" << GetLastError() << std::endl;
	}

	// set font size
	CONSOLE_FONT_INFOEX ConsoleFontInfo;
	ConsoleFontInfo.dwFontSize.X = 8;                   // Width 
	ConsoleFontInfo.dwFontSize.Y = 12;                  // Height
	SetCurrentConsoleFontEx(hConsole, FALSE, &ConsoleFontInfo);


	// fill up array
	Screen.resize(ScreenX, std::vector<wchar_t>(ScreenY, L' '));

	for (int i = 0; i < ScreenX; i++)
		for (int j = 0; j < ScreenY; j++)
			Screen.at(i).at(j) = L' ';
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
		UpdateF();
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