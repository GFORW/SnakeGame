#pragma once
#include <Windows.h>
#include <thread>
#include <chrono>
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


	int ScreenX;
	int ScreenY;

	int GameSpeed;
	int play;
private:
	void Render();


	HANDLE hConsole;
	HANDLE hConsoleIn;

	std::vector<std::vector<wchar_t>> Screen;

	COORD windowBufSize;
	COORD DwBufferCoord;
	SMALL_RECT WindowSize;
};



