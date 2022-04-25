#pragma once
#include <vector>
#include <iostream>

enum Direction
{
	left,
	up,
	right,
	down
};


class Coord // Change to COORD
{
public:
	Coord(int X, int Y);
	Coord();
	~Coord();
	int x;
	int y;
};

class Snake
{
public:
	Direction dir;
	Snake(int x = 1 , int y =1);
	~Snake();
	void addPiece(int x, int y);
	void move();
	Coord getCoord(int i);
	int getLength();
	std::unique_ptr<std::vector<Coord>> ptrBody;
	std::unique_ptr<std::vector<Coord>> ptrOldBody;

private:
	std::vector<Coord> body;
	std::vector<Coord> oldbody;
};


