#pragma once
#include "snake.h"
#include <vector>

Snake::Snake(int x, int y)  // set starting position
{
	body.push_back(Coord(x,y));
	body.push_back(Coord(x, y-1));

	oldbody = body;
	ptrBody = std::make_unique<std::vector<Coord>>(body);
	ptrOldBody = std::make_unique<std::vector<Coord>>(oldbody);;
}

Snake::~Snake()
{
}

void Snake::addPiece()
{
	ptrBody->erase(ptrBody->begin() + 1, ptrBody->end());  // preallocate memory
	ptrBody->reserve(ptrBody->size() + ptrBody->size());
	ptrBody->insert(ptrBody->end(), ptrOldBody->begin(), ptrOldBody->end());
}

void Snake::move()
{
	Coord* head = ptrBody->data();
	*ptrOldBody = *ptrBody;
	switch (dir)
	{
	case left:
		head->x -= 1;
		break;
	case up:
		head->y -= 1;
		break;
	case right:
		head->x += 1;
		break;
	case down:
		head->y += 1;
		break;
	}

	// move tail
	for (size_t i = 1; i < ptrBody->size(); i++)
	{
		ptrBody->at(i) = ptrOldBody->at(i-1);
	}
}

Coord Snake::getCoord(int i)
{
	return body.at(i);
}

int Snake::getLength()
{
	return body.size();
}


////
//// Coord Struct
////


Coord::Coord(int X, int Y)
{
	x = X;
	y = Y;
}

Coord::Coord()
{
	x = 0;
	y = 0;
}

Coord::~Coord()
{
}