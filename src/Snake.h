#pragma once
#include <vector>
#include <iostream>
#include <Windows.h>

enum Direction
{
	left,
	up,
	right,
	down
};

class Snake
{
public:
	Snake(const int& x = 5 , const int& y = 5);
	~Snake();

	void addPiece();

	std::unique_ptr<std::vector<COORD>> ptrBody;
	std::unique_ptr<std::vector<COORD>> ptrOldBody;

	Direction dir = down;
	Direction prev_dir = down;
};