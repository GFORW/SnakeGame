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
	Direction dir;
	Direction prev_dir;

	Snake(int x = 5 , int y = 5);
	~Snake();

	void addPiece();

	std::unique_ptr<std::vector<COORD>> ptrBody;
	std::unique_ptr<std::vector<COORD>> ptrOldBody;

};


