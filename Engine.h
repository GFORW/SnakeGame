#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>


class Engine
{
public:
	Engine(int X = 50, int Y = 25);
	virtual ~Engine();


	virtual void KeyPressed(int Code) {};
	virtual void Update() {};


	void SetChar(int x, int y, wchar_t c) ;
	void Run();
	wchar_t GetChar(int x, int y);
	void ChangeSpeed(int sec);
	virtual void GameOver() {};


	int ScreenX;
	int ScreenY;

	bool MENU;
	bool GAME_OVER = 0;
private:
	void Render();
	int GameSpeed = 60;


	HANDLE hConsole;
	HANDLE hConsoleIn;

	std::vector<std::vector<wchar_t>> Screen;

	COORD windowBufSize;
	COORD DwBufferCoord;
	SMALL_RECT WindowSize;
};



